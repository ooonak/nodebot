#pragma once

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

  // TODO Replace id
  uint64_t mId{123};
  dpp::message mEmbedMessage;

  void onMessageCreate(const dpp::message_create_t &event);
  void onMessageUpdate(const dpp::message_update_t &event);
};

}  // namespace nb
