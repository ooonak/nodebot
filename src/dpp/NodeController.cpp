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
      // New node, create first message (and later thread) with informal embed.
      dpp::embed tmp;
      tmp.set_color(dpp::colors::green);
      tmp.set_title(node.info.name);
      tmp.set_description(node.info.description);

      for (const auto &detail : node.info.details)
      {
        tmp.add_field(detail.first, detail.second, true);
      }

      tmp.add_field("Created", ISO8601UTC(node.created));
      tmp.set_footer(Needle + std::to_string(node.id), "");

      mNodes[node.id] = {nullptr, nullptr};
      mBot->message_create(dpp::message(channelId, tmp));
    }
    else if (node.created != node.lastActive)
    {
      if (mNodes[node.id].message != nullptr)
      {
        mNodes[node.id].message->embeds[0].set_title(node.info.name);
        mNodes[node.id].message->embeds[0].set_description(
            node.info.description);
        mNodes[node.id].message->embeds[0].fields.clear();
        for (const auto &detail : node.info.details)
        {
          mNodes[node.id].message->embeds[0].add_field(detail.first,
                                                       detail.second, true);
        }
        mNodes[node.id].message->embeds[0].add_field("Created",
                                                     ISO8601UTC(node.created));
        mNodes[node.id].message->embeds[0].add_field(
            "Last active", ISO8601UTC(node.lastActive));
        mBot->message_edit(*mNodes[node.id].message);
      }
    }
  }
}

void nb::NodeController::onMessageCreate(const dpp::message_create_t &event)
{
  const uint64_t id = idFromMessage(event.msg);
  if (id != 0 && mNodes.find(id) != mNodes.end())
  {
    mNodes[id].message = std::make_unique<dpp::message>(event.msg);
    if (mNodes[id].thread == nullptr)
    {
      // First message for this node id, create thread.
      mLogger->info(
          "Created new embed message, node id {}, message snowflake {}", id,
          mNodes[id].message->id);
      mBot->thread_create_with_message(
          mNodes[id].message->embeds[0].title, mNodes[id].message->channel_id,
          mNodes[id].message->id, 1440, 0,
          std::bind(&NodeController::onThreadCreate, this, _1));
    }
  }
}

void nb::NodeController::onThreadCreate(
    const dpp::confirmation_callback_t &event)
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
      // Message and thread snowflake is the same.
      if (node.thread == nullptr && node.message != nullptr &&
          std::get<dpp::thread>(event.value).id == node.message->id)
      {
        node.thread =
            std::make_unique<dpp::thread>(std::get<dpp::thread>(event.value));
        mLogger->info(
            "Created new thread, node id {}, thread snowflake {}, channel "
            "snowflake {}.",
            node.thread->name, node.thread->id, node.thread->parent_id);
      }
    }
  }
}

void nb::NodeController::onMessageUpdate(const dpp::message_update_t &event)
{
  const uint64_t id = idFromMessage(event.msg);
  if (id != 0 && mNodes.find(id) != mNodes.end())
  {
    mNodes[id].message = std::make_unique<dpp::message>(event.msg);
    mLogger->info("Message updated, node id {}, message snowflake {}", id,
                  mNodes[id].message->id);
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

uint64_t nb::NodeController::idFromMessage(const dpp::message &msg) const
{
  uint64_t id = 0;

  try
  {
    for (const auto embed : msg.embeds)
    {
      if (const auto footer = embed.footer)
      {
        const auto idPos = (*footer).text.find(Needle);
        if (idPos != std::string::npos)
        {
          const auto idStr = (*footer).text.substr(idPos + Needle.length());
          id = std::stoull(idStr, nullptr, 10);
        }
      }
    }
  }
  catch (const std::exception &exc)
  {
    mLogger->error("{} {}", __func__, exc.what());
  }

  return id;
}
