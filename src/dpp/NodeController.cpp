#include <sstream>
#include <iomanip>
#include "NodeController.hpp"

using namespace std::placeholders;

static std::string Needle{"ID: "};

nb::NodeController::NodeController(std::shared_ptr<dpp::cluster> bot, const std::string& botName, const std::string& botDescription)
    : mBot{bot}, mLogger{spdlog::get("DPP")}, mBotName{botName}, mBotDescription{botDescription}
{
  mStarted = std::chrono::system_clock::now();
  mBot->on_message_create(
      std::bind(&NodeController::onMessageCreate, this, _1));
  mBot->on_message_update(
      std::bind(&NodeController::onMessageUpdate, this, _1));
}

void nb::NodeController::update(dpp::snowflake channelId, const std::vector<nb::NodeInfo> &nodesInfo)
{
  // TODO What if channel has changed?
  mEmbedMessage.channel_id = channelId;
  mEmbedMessage.embeds.clear();

  auto botEmbed = dpp::embed();
  botEmbed.set_color(dpp::colors::sti_blue);
  botEmbed.set_title(mBotName);
  botEmbed.set_description(mBotDescription);
  botEmbed.add_field("Updated", ISO8601UTC(mStarted));
  botEmbed.set_footer(Needle + std::to_string(mId), "");
  mEmbedMessage.embeds.push_back(botEmbed);

  for (const auto nodeInfo : nodesInfo)
  {
    auto embed = dpp::embed();
    embed.set_color(getColor());
    embed.set_title(nodeInfo.name);
    embed.set_description(nodeInfo.description);
    for (const auto detail : nodeInfo.details)
    {
      embed.add_field(detail.first, detail.second, true);
    }
    embed.set_footer("Created: " + ISO8601UTC(nodeInfo.created), "");

    mEmbedMessage.embeds.push_back(embed);
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

uint32_t nb::NodeController::getColor()
{
  char temp = mRed;
  mRed = mGreen;
  mGreen = temp;
  mRed += 25;
  mGreen -= 25;
  mBlue += mRed + mGreen;
  return (255<<24) + (int(mRed)<<16) + (int(mGreen)<<8) + int(mBlue);
}

std::string nb::NodeController::ISO8601UTC(const std::chrono::system_clock::time_point& timep) const
{
  const auto then = std::chrono::system_clock::to_time_t(timep);
  std::ostringstream ss;
  ss << std::put_time(gmtime(&then), "%FT%TZ");
  return ss.str();
}
