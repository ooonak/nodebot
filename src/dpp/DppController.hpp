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

namespace nb
{

class DppController
{
 public:
  explicit DppController(const nb::Config &config,
                         std::shared_ptr<nb::NodeQueues> nodeQueues);

  ~DppController();

  void start();

  void stop();

 private:
  const nb::Config &mConfig;
  std::shared_ptr<spdlog::logger> mLogger;

  std::shared_ptr<dpp::cluster> mBot;
  std::unique_ptr<dpp::guild> mGuild;
  dpp::snowflake mChannelId;

  dpp::timer mTimer;
  dpp::timer mMessageTimer;

  std::atomic<bool> mStop{false};

  std::unique_ptr<nb::ChannelController> mChannelController;
  std::unique_ptr<nb::NodeController> mNodeController;
  std::unique_ptr<nb::SlashCommandController> mSlashCommandController;
  std::shared_ptr<nb::NodeQueues> mNodeQueues;
  nb::NodeHandlesT mNodes;

  enum class State
  {
    Init,
    WaitingForChannels,
    WaitingForThreads,
    Ready
  };

  State mState{State::Init};

  void onGetGuilds(const dpp::confirmation_callback_t &event);

  void onTimerTick();

  void onMessageTimerTick();
};

}  // namespace nb
