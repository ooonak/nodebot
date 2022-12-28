#pragma once

#include <memory>
#include <unordered_map>
#include <tuple>

#include "NodeQueues.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class NodeController
{
 public:
  explicit NodeController(std::shared_ptr<dpp::cluster> bot);

  void update(dpp::snowflake channelId, const nb::NodeHandlesT &nodes);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;

  struct NodeT {
    uint64_t nodeHandleId;
    std::unique_ptr<dpp::thread> thread;
    std::unique_ptr<dpp::message> message;
  };

  std::unordered_map<uint64_t, NodeT> mNodes;

  void onThreadCreate(const dpp::confirmation_callback_t &event);

  void onMessageCreate(const dpp::message_create_t &event);
  void onMessageUpdate(const dpp::message_update_t &event);

  static std::string ISO8601UTC(
      const std::chrono::system_clock::time_point &tp);
};

}  // namespace nb
