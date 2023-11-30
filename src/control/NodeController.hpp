#pragma once

#include <memory>
#include <tuple>
#include <unordered_map>

#include "NodeQueues.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace ok
{

class NodeController
{
 public:
  explicit NodeController(std::shared_ptr<dpp::cluster> bot, dpp::snowflake channelId, const std::shared_ptr<spdlog::logger>& logger);

  bool ready() const;

  bool errorOccured() const;

  void update(dpp::snowflake channelId, const ok::NodeHandlesT &nodes);

  dpp::snowflake threadId(uint64_t id);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  dpp::snowflake mChannelId;
  std::shared_ptr<spdlog::logger> mLogger;

  bool mReady{false};
  bool mErrorOccured{false};

  struct NodeT
  {
    // uint64_t nodeHandleId;
    std::unique_ptr<dpp::thread> thread;
    std::unique_ptr<dpp::message> message;
  };

  std::unordered_map<uint64_t, NodeT> mNodes;

  void onThreadsList(const dpp::confirmation_callback_t &event);

  void onThreadCreate(const dpp::confirmation_callback_t &event);

  void onMessageCreate(const dpp::message_create_t &event);

  void onMessageUpdate(const dpp::message_update_t &event);

  static std::string ISO8601UTC(const std::chrono::system_clock::time_point &tp);

  /**
   * @brief idFromMessage Extracts id from footer in embed.
   * @param msg
   * @return 0 if not found else id.
   */
  uint64_t idFromMessage(const dpp::message &msg) const;
};

}  // namespace NodeBot
