#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>

#include "ChannelController.hpp"

using namespace std::placeholders;

nb::ChannelController::ChannelController(std::shared_ptr<dpp::cluster> bot, std::string channelPrefix, int channelLifetimeInHours)
    : mBot{bot}, mLogger{spdlog::get("DPP")}, mPrefix{channelPrefix}, mLifetimeHours{channelLifetimeInHours}
{

}

void nb::ChannelController::start(dpp::snowflake guildId)
{
  mBot->channels_get(guildId, std::bind(&ChannelController::onChannelsGet, this, _1));
}

void nb::ChannelController::onChannelsGet(const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    auto channels = std::get<dpp::channel_map>(event.value);
    dpp::snowflake newestChannelId{0};

    for (auto const &[key, value] : channels)
    {
      if (value.get_type() == dpp::CHANNEL_TEXT && value.name.starts_with(mPrefix))
      {
        bool channelExpired = channelOlderThan(value, mLifetimeHours);
        if (channelExpired)
        {
          mExpiredChannelsToDelete.push_back(value);
        }
        else
        {
          if (newestChannelId == dpp::snowflake{0})
          {
            newestChannelId = key;
          }
          else if (channelCreatedAfter(value, channels.at(newestChannelId)))
          {
            newestChannelId = key;
          }
        }
      }
    }

    if (newestChannelId != dpp::snowflake{0})
    {
      mActiveChannel = std::make_unique<dpp::channel>(channels.at(newestChannelId));
      mLogger->info("Setting active channel to {}, id: {}, created: {}.", mActiveChannel->name, mActiveChannel->id, ISO8601UTC(mActiveChannel->id));
    }
    else
    {
      //mLogger->info("Created new channel {}, id: {}, created: {}.", mActiveChannel->name, mActiveChannel->id, ISO8601UTC(mActiveChannel->id));
    }
  }
}

std::string nb::ChannelController::ISO8601UTC(dpp::snowflake id)
{
  const auto now = std::chrono::system_clock::now();
  const auto then = std::chrono::system_clock::to_time_t(now - std::chrono::milliseconds((id >> 22) +1420070400000));
  std::ostringstream ss;
  ss << std::put_time(gmtime(&then), "%FT%TZ");
  return ss.str();
}

bool nb::ChannelController::channelOlderThan(const dpp::channel &channel, int channelLifetimeInHours)
{
  const auto now = dpp::utility::uptime(dpp::utility::time_f());
  const auto created = dpp::utility::uptime(channel.get_creation_time());
  const auto delta = now.to_secs() - created.to_secs();

  if (delta > channelLifetimeInHours * 3600)
  {
    mLogger->info("Channel {}, id: {}, created: {}, more than {} hours old ({} seconds).", channel.name, channel.id, ISO8601UTC(channel.id), channelLifetimeInHours, delta);
    return true;
  }
  else
  {
    return false;
  }
}

bool nb::ChannelController::channelCreatedAfter(const dpp::channel &channel, const dpp::channel &compareAgainst)
{
  const auto channelCreated = dpp::utility::uptime(channel.get_creation_time());
  const auto compareAgainstCreated = dpp::utility::uptime(compareAgainst.get_creation_time());
  const auto delta = channelCreated.to_secs() - compareAgainstCreated.to_secs();
  return (delta < 0);
}
