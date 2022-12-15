#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class ChannelController
{
 public:
  explicit ChannelController(std::shared_ptr<dpp::cluster> bot,
                             std::string realm, std::string subRealm,
                             int channelLifetimeInHours);

  void start(dpp::snowflake guildId);

  bool ready(dpp::snowflake &id) const;

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;

  const std::string mRealm;
  const std::string mSubRealm;
  const int mLifetimeHours;

  std::unique_ptr<dpp::channel> mActiveChannel;
  std::vector<dpp::channel> mExpiredChannelsToDelete;
  size_t mChannelsDeleted{0};

  dpp::snowflake mGuildId{0};

  bool mReady{false};

  void onChannelsGet(const dpp::confirmation_callback_t &event);
  void onChannelCreate(const dpp::confirmation_callback_t &event);
  void onChannelDelete(const dpp::confirmation_callback_t &event);

  // Helpers
  static std::string ISO8601UTC(dpp::snowflake id);

  bool channelOlderThan(const dpp::channel &channel,
                        int channelLifetimeInHours);

  static bool channelCreatedAfter(const dpp::channel &channel,
                                  const dpp::channel &compareAgainst);
};

}  // namespace nb
