#include "NodeController.hpp"

using namespace std::placeholders;

static std::string Needle{"ID: "};

nb::NodeController::NodeController(std::shared_ptr<dpp::cluster> bot, const std::string& botName, const std::string& botDescription)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
  mEmbedMessage.embeds.push_back(dpp::embed());
  mEmbedMessage.embeds[0].set_color(dpp::colors::sti_blue);
  mEmbedMessage.embeds[0].set_title(botName);
  mEmbedMessage.embeds[0].set_description(botDescription);
  mEmbedMessage.embeds[0].set_footer(Needle + std::to_string(mId), "");

  mBot->on_message_create(
      std::bind(&NodeController::onMessageCreate, this, _1));
  mBot->on_message_update(
      std::bind(&NodeController::onMessageUpdate, this, _1));
}

void nb::NodeController::update(dpp::snowflake channelId, const std::vector<nb::NodeInfo> &nodesInfo)
{
  // TODO What if channel has changed?
  mEmbedMessage.channel_id = channelId;

  mEmbedMessage.embeds[0].fields.clear();
  for (const auto nodeInfo : nodesInfo)
  {
    mEmbedMessage.embeds[0].add_field(nodeInfo.name, nodeInfo.description);
  }

  if (mEmbedMessage.id.empty())
  {
    mBot->message_create(mEmbedMessage);
  }
  else
  {
    mBot->message_edit(mEmbedMessage);
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
        if (id != 0 && id == mId)
        {
          mEmbedMessage = event.msg;
          mLogger->info("Message created, mapped message id {} -> snowflake {}", id, mEmbedMessage.id);
        }
      }
    }
  }
}

void nb::NodeController::onMessageUpdate(const dpp::message_update_t &event)
{
  mLogger->debug(__func__);
}
