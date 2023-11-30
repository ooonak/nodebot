#pragma once

#include <memory>

#include "InternalDataTypes.hpp"
#include "NodeQueues.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class WebHookController
{
 public:
  explicit WebHookController(std::shared_ptr<dpp::cluster> bot,
                             nb::NodeHandlesT &nodes,
                             std::shared_ptr<nb::NodeQueues> nodeQueues);

  void createWebHook(dpp::snowflake guildId, dpp::snowflake channelId,
                     uint64_t nodeId);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  nb::NodeHandlesT &mNodes;
  std::shared_ptr<nb::NodeQueues> mNodeQueues;
  std::shared_ptr<spdlog::logger> mLogger;

  void onCreateWebHook(const dpp::confirmation_callback_t &event);
};

}  // namespace NodeBot
