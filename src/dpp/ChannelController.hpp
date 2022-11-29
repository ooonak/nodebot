#pragma once

#include <memory>
#include <string>

#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class ChannelController
{
 public:
  explicit ChannelController(std::shared_ptr<dpp::cluster> bot, std::string channelPrefix, int channelLifetimeInHours);

  void start(dpp::snowflake guildId);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;

  const std::string mPrefix;
  const int mLifetimeHours;

  std::unique_ptr<dpp::channel> mActiveChannel;

  void onChannelsGet(const dpp::confirmation_callback_t &event);
};

}  // namespace nb
