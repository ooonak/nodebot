#include "NodeController.hpp"
#include <sstream>

using namespace std::placeholders;

static std::string Needle{"ID: "};

nb::NodeController::NodeController(std::shared_ptr<dpp::cluster> bot, const std::string& name, const std::string &description)
    : mBot{bot}, mLogger{spdlog::get("DPP")}, mName{name}, mDescription{description}
{
  mBot->on_message_create(
      std::bind(&NodeController::onMessageCreate, this, _1));
  mBot->on_message_update(
      std::bind(&NodeController::onMessageUpdate, this, _1));
}

void nb::NodeController::update(dpp::snowflake channelId, const nb::NodeQueues::NodeHandlesT &nodes)
{
  for (const auto &node : nodes)
  {
    if (mNodes.find(node.id) == mNodes.end())
    {
      dpp::embed tmp;
      tmp.set_color(dpp::colors::sti_blue);
      tmp.set_title(node.info.name);
      tmp.set_description(node.info.description);

      for (const auto &detail : node.info.details)
      {
        tmp.add_field(detail.first, detail.second, true);
      }

      tmp.add_field("Created", ISO8601UTC(node.created));
      tmp.set_footer(Needle + std::to_string(node.id), "");

      mNodes[node.id] = dpp::message(channelId, tmp);
      mBot->message_create(mNodes[node.id]);
    }
    else
    {
      mNodes[node.id].embeds[0].set_title(node.info.name);
      mNodes[node.id].embeds[0].set_description(node.info.description);

      mNodes[node.id].embeds[0].fields.clear();
      for (const auto &detail : node.info.details)
      {
        mNodes[node.id].embeds[0].add_field(detail.first, detail.second, true);
      }

      mNodes[node.id].embeds[0].add_field("Created", ISO8601UTC(node.created));
      mNodes[node.id].embeds[0].add_field("Last active", ISO8601UTC(node.lastActive));

      mBot->message_edit(mNodes[node.id]);
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
          mNodes[id].id = event.msg.id;
          mLogger->info("Message created, mapped message id {} -> snowflake {}", id, mNodes[id].id);
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
          mLogger->info("Message updated, message id {} -> snowflake {}", id, mNodes[id].id);
        }
      }
    }
  }
}

std::string nb::NodeController::ISO8601UTC(const std::chrono::system_clock::time_point &tp)
{
  const auto then = std::chrono::system_clock::to_time_t(tp);
  std::ostringstream ss;
  ss << std::put_time(gmtime(&then), "%FT%TZ");
  return ss.str();
}
