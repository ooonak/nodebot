#include "ChannelController.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std::placeholders;

nb::ChannelController::ChannelController(std::shared_ptr<dpp::cluster> bot,
                                         std::string realm, std::string subRealm,
                                         int channelLifetimeInHours)
    : mBot{bot},
      mLogger{spdlog::get("DPP")},
      mRealm{realm},
      mSubRealm{subRealm},
      mLifetimeHours{channelLifetimeInHours}
{
}

void nb::ChannelController::start(dpp::snowflake guildId)
{
  mGuildId = guildId;
  mBot->channels_get(mGuildId,
                     std::bind(&ChannelController::onChannelsGet, this, _1));
}

bool nb::ChannelController::ready(dpp::snowflake &id) const
{
  if (mReady == true && mActiveChannel != nullptr)
  {
    id = mActiveChannel->id;
    return true;
  }
  else
  {
    return false;
  }
}

void nb::ChannelController::onChannelsGet(
    const dpp::confirmation_callback_t &event)
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
      /*
      if (value.is_category())
      {
        // TODO: Get or create category channel first and use parent id for sub
      channels. mLogger->info("{} {}", value.name, value.id);
      }
      */

      if (value.get_type() == dpp::CHANNEL_TEXT &&
          value.name.starts_with(mRealm))
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
      mActiveChannel =
          std::make_unique<dpp::channel>(channels.at(newestChannelId));
      mLogger->info("Setting active channel to {}, id: {}, created: {}.",
                    mActiveChannel->name, mActiveChannel->id,
                    ISO8601UTC(mActiveChannel->id));
      if (mExpiredChannelsToDelete.size() == mChannelsDeleted)
      {
        mLogger->debug("Setting ChannelController ready.");
        mReady = true;
      }
    }
    else
    {
      dpp::channel newChannel;
      std::string name = mRealm;
      const auto now = std::chrono::system_clock::now();
      const auto itt = std::chrono::system_clock::to_time_t(now);
      std::ostringstream ss;
      ss << mRealm << "-" << std::put_time(gmtime(&itt), "%Y%m%d%H%M");

      newChannel.set_name(ss.str());
      newChannel.set_guild_id(mGuildId);
      newChannel.set_type(dpp::channel_type::CHANNEL_TEXT);

      mBot->channel_create(
          newChannel, std::bind(&ChannelController::onChannelCreate, this, _1));
    }

    if (!mExpiredChannelsToDelete.empty())
    {
      for (const auto channel : mExpiredChannelsToDelete)
      {
        mLogger->debug("About to delete channel {}, id: {}", channel.name,
                       channel.id);
        mBot->channel_delete(
            channel.id,
            std::bind(&ChannelController::onChannelDelete, this, _1));
      }
    }
  }
}

void nb::ChannelController::onChannelCreate(
    const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    mActiveChannel =
        std::make_unique<dpp::channel>(std::get<dpp::channel>(event.value));
    mLogger->info("Created new channel {}, id: {}.", mActiveChannel->name,
                  mActiveChannel->id);
    if (mExpiredChannelsToDelete.size() == mChannelsDeleted)
    {
      mLogger->debug("Setting ChannelController ready.");
      mReady = true;
    }
  }
}

void nb::ChannelController::onChannelDelete(
    const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    mLogger->info("Deleted channel.");
    mChannelsDeleted++;
    if (mExpiredChannelsToDelete.size() == mChannelsDeleted)
    {
      spdlog::debug("Setting ChannelController ready.");
      mReady = true;
    }
  }
}

std::string nb::ChannelController::ISO8601UTC(dpp::snowflake id)
{
  const auto now = std::chrono::system_clock::now();
  const auto then = std::chrono::system_clock::to_time_t(
      now - std::chrono::milliseconds((id >> 22) + 1420070400000));
  std::ostringstream ss;
  ss << std::put_time(gmtime(&then), "%FT%TZ");
  return ss.str();
}

bool nb::ChannelController::channelOlderThan(const dpp::channel &channel,
                                             int channelLifetimeInHours)
{
  const auto now = dpp::utility::uptime(dpp::utility::time_f());
  const auto created = dpp::utility::uptime(channel.get_creation_time());
  const auto delta = now.to_secs() - created.to_secs();

  if (delta > channelLifetimeInHours * 3600)
  {
    mLogger->info(
        "Channel {}, id: {}, created: {}, more than {} hours old ({} seconds).",
        channel.name, channel.id, ISO8601UTC(channel.id),
        channelLifetimeInHours, delta);
    return true;
  }
  else
  {
    return false;
  }
}

bool nb::ChannelController::channelCreatedAfter(
    const dpp::channel &channel, const dpp::channel &compareAgainst)
{
  const auto channelCreated = dpp::utility::uptime(channel.get_creation_time());
  const auto compareAgainstCreated =
      dpp::utility::uptime(compareAgainst.get_creation_time());
  const auto delta = channelCreated.to_secs() - compareAgainstCreated.to_secs();
  return (delta < 0);
}
