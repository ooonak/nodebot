#pragma once

#include <memory>

#include "ChannelController.hpp"
#include "Config.hpp"
#include "NodeController.hpp"
#include "SlashCommandController.hpp"
#include "WebHookController.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class DppController
{
 public:
  explicit DppController(const nb::Config &config);

  ~DppController();

 private:
  const nb::Config &mConfig;
  std::shared_ptr<spdlog::logger> mLogger;

  std::shared_ptr<dpp::cluster> mBot;
  std::unique_ptr<dpp::guild> mGuild;
  dpp::timer mTimer;

  std::unique_ptr<nb::ChannelController> mChannelController;
  std::unique_ptr<nb::NodeController> mNodeController;
  std::unique_ptr<nb::SlashCommandController> mSlashCommandController;
  std::unique_ptr<nb::WebHookController> mWebHookController;

  void onGetGuilds(const dpp::confirmation_callback_t &event);

  void onTimerTick();
};

}  // namespace nb
