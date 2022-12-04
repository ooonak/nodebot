#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "nb/DataTypes.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class NodeController
{
 public:
  explicit NodeController(std::shared_ptr<dpp::cluster> bot, const std::string& botName, const std::string& botDescription);

  void update(dpp::snowflake channelId, const std::vector<nb::NodeInfo> &nodesInfo);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;

  const std::string mBotName;
  const std::string mBotDescription;

  uint64_t mId{123};
  dpp::message mEmbedMessage;

  std::chrono::system_clock::time_point mStarted;

  char mRed{0};
  char mGreen{0};
  char mBlue{0};

  void onMessageCreate(const dpp::message_create_t &event);
  void onMessageUpdate(const dpp::message_update_t &event);

  uint32_t getColor();

  std::string ISO8601UTC(const std::chrono::system_clock::time_point& timep) const;
};

}  // namespace nb
