#pragma once

#include <atomic>
#include <memory>

#include "ChannelController.hpp"
#include "Config.hpp"
#include "NodeController.hpp"
#include "NodeQueues.hpp"
#include "SlashCommandController.hpp"
#include "WebHookController.hpp"
#include "dpp/dpp.h"
#include "dpp/InternalDataTypes.hpp"
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
  dpp::timer mTimer;

  std::atomic<bool> mStop{false};

  std::unique_ptr<nb::ChannelController> mChannelController;
  std::unique_ptr<nb::NodeController> mNodeController;
  std::unique_ptr<nb::SlashCommandController> mSlashCommandController;
  std::unique_ptr<nb::WebHookController> mWebHookController;
  std::shared_ptr<nb::NodeQueues> mNodeQueues;
  nb::NodeHandlesT mNodes;

  void onGetGuilds(const dpp::confirmation_callback_t &event);

  void onTimerTick();
};

}  // namespace nb
