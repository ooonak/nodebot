#include "NodeController.hpp"

#include <sstream>

using namespace std::placeholders;

static std::string Needle{"ID: "};

nb::NodeController::NodeController(std::shared_ptr<dpp::cluster> bot)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
  mBot->on_message_create(
      std::bind(&NodeController::onMessageCreate, this, _1));
  mBot->on_message_update(
      std::bind(&NodeController::onMessageUpdate, this, _1));
}

void nb::NodeController::update(dpp::snowflake channelId,
                                const nb::NodeHandlesT &nodes)
{
  for (const auto &node : nodes)
  {
    if (mNodes.find(node.id) == mNodes.end())
    {
      // Just prepare the message, we send it on the thread later.
      dpp::embed tmp;
      tmp.set_color(dpp::colors::red);
      tmp.set_title(node.info.name);
      tmp.set_description(node.info.description);

      for (const auto &detail : node.info.details)
      {
        tmp.add_field(detail.first, detail.second, true);
      }

      tmp.add_field("Created", ISO8601UTC(node.created));
      tmp.set_footer(Needle + std::to_string(node.id), "");

      mNodes[node.id] = { node.id, nullptr, std::make_unique<dpp::message>(dpp::snowflake(), tmp) };

      // 1440 = Duration after which thread auto-archives.
      // 0 = Amount of seconds a user has to wait before sending another message.
      mBot->thread_create(std::to_string(node.id), channelId, 1440, dpp::CHANNEL_PRIVATE_THREAD, false, 0, std::bind(&NodeController::onThreadCreate, this, _1));
    }
    else if (node.created != node.lastActive)
    {
      /*
      mNodes[node.id].embeds[0].set_color(dpp::colors::green);

      mNodes[node.id].embeds[0].set_title(node.info.name);
      mNodes[node.id].embeds[0].set_description(node.info.description);

      mNodes[node.id].embeds[0].fields.clear();
      for (const auto &detail : node.info.details)
      {
        mNodes[node.id].embeds[0].add_field(detail.first, detail.second, true);
      }

      mNodes[node.id].embeds[0].add_field("Created", ISO8601UTC(node.created));
      mNodes[node.id].embeds[0].add_field("Last active",
                                          ISO8601UTC(node.lastActive));

      mBot->message_edit(mNodes[node.id]);
      */
    }
  }
}

void nb::NodeController::onThreadCreate(const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    for (auto &pair : mNodes)
    {
      auto &node = pair.second;
      if (node.thread == nullptr && node.message != nullptr)
      {
        node.thread = std::make_unique<dpp::thread>(std::get<dpp::thread>(event.value));
        node.message->set_channel_id(node.thread->id);
        mLogger->info("Created new thread {} in channel {} for node {}.", node.thread->id, node.thread->parent_id, node.thread->name);
        mBot->message_create(*node.message);
      }
    }
  }
}

void nb::NodeController::onMessageCreate(const dpp::message_create_t &event)
{
  for (const auto embed : event.msg.embeds)
  {
    if (const auto footer = embed.footer)
    {
      const auto idPos = (*footer).text.find(Needle);
      if (idPos != std::string::npos)
      {
        const auto idStr = (*footer).text.substr(idPos + Needle.length());
        const uint64_t id = std::stoull(idStr, nullptr, 10);
        if (id != 0 && (mNodes.find(id) != mNodes.end()))
        {
          mNodes[id].message = std::make_unique<dpp::message>(event.msg);
          mLogger->info("Message created, mapped message id {} -> snowflake {}", id, mNodes[id].message->id);
        }
      }
    }
  }
}

void nb::NodeController::onMessageUpdate(const dpp::message_update_t &event)
{
  for (const auto embed : event.msg.embeds)
  {
    if (const auto footer = embed.footer)
    {
      const auto idPos = (*footer).text.find(Needle);
      if (idPos != std::string::npos)
      {
        const auto idStr = (*footer).text.substr(idPos + Needle.length());
        const uint64_t id = std::stoull(idStr, nullptr, 10);
        if (id != 0 && (mNodes.find(id) != mNodes.end()))
        {
          mLogger->info("Message updated, message id {} -> snowflake {}", id, mNodes[id].nodeHandleId);
        }
      }
    }
  }
}

std::string nb::NodeController::ISO8601UTC(
    const std::chrono::system_clock::time_point &tp)
{
  const auto then = std::chrono::system_clock::to_time_t(tp);
  std::ostringstream ss;
  ss << std::put_time(gmtime(&then), "%FT%TZ");
  return ss.str();
}
