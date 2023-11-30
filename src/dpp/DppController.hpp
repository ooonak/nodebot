#pragma once

#include <atomic>
#include <memory>
#include "ChannelController.hpp"
#include "Config.hpp"
#include "NodeController.hpp"
#include "NodeQueues.hpp"
#include "SlashCommandController.hpp"
#include "dpp/InternalDataTypes.hpp"
#include "dpp/dpp.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

namespace ok
{

class DppController
{
 public:
  explicit DppController(const ok::Config &config,
                         std::shared_ptr<ok::NodeQueues> nodeQueues,
                         const std::shared_ptr<spdlog::logger>& logger);

  ~DppController();

  void start();

  void stop();

 private:
  const ok::Config &mConfig;
  std::shared_ptr<spdlog::logger> mLogger;

  std::shared_ptr<dpp::cluster> mBot;
  std::unique_ptr<dpp::guild> mGuild;
  dpp::snowflake mChannelId;

  dpp::timer mTimer;
  dpp::timer mMessageTimer;

  std::atomic<bool> mStop{false};

  std::unique_ptr<ok::ChannelController> mChannelController;
  std::unique_ptr<ok::NodeController> mNodeController;
  std::unique_ptr<ok::SlashCommandController> mSlashCommandController;
  std::shared_ptr<ok::NodeQueues> mNodeQueues;
  ok::NodeHandlesT mNodes;

  enum class State
  {
    Init,
    WaitingForChannels,
    WaitingForThreads,
    Ready
  };

  State mState{State::Init};

  /**
   * @brief onGetGuilds: Callack is called when we are connected and get information about the Discord server.
   * @param event
   */
  void onGetGuilds(const dpp::confirmation_callback_t &event);

  /**
   * @brief onTimerTick: Drives state machine.
   */
  void onTimerTick();

  /**
   * @brief onMessageTimerTick: Polls and sends messages from queue if any.
   */
  void onMessageTimerTick();
};

}  // namespace NodeBot
