#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class ChannelController
{
 public:
  explicit ChannelController(std::shared_ptr<dpp::cluster> bot, std::string channelPrefix, int channelLifetimeInHours);

  void start(dpp::snowflake guildId);

  std::optional<dpp::snowflake> ready() const;

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;

  const std::string mPrefix;
  const int mLifetimeHours;

  std::unique_ptr<dpp::channel> mActiveChannel;
  std::vector<dpp::channel> mExpiredChannelsToDelete;

  dpp::snowflake mGuildId{0};

  bool mReady{false};

  void onChannelsGet(const dpp::confirmation_callback_t &event);
  void onChannelCreate(const dpp::confirmation_callback_t &event);
  void onChannelDelete(const dpp::confirmation_callback_t &event);

  // Helpers
  static std::string ISO8601UTC(dpp::snowflake id);

  bool channelOlderThan(const dpp::channel &channel, int channelLifetimeInHours);

  static bool channelCreatedAfter(const dpp::channel &channel, const dpp::channel &compareAgainst);
};

}  // namespace nb
