#include "dpp/Controller.hpp"

#include <iostream>

nb::Controller::Controller(const nb::Config &config) : mConfig{config}, mLogger{spdlog::get("DPP")}
{
  mLogger->debug("{} {}", __func__, mConfig.token);

  dpp::cluster bot(mConfig.token);

  bot.on_log([&bot, this](const dpp::log_t & event) {
    switch (event.severity) {
      case dpp::ll_trace:
        mLogger->trace("{}", event.message);
        break;
      case dpp::ll_debug:
        mLogger->debug("{}", event.message);
        break;
      case dpp::ll_info:
        mLogger->info("{}", event.message);
        break;
      case dpp::ll_warning:
        mLogger->warn("{}", event.message);
        break;
      case dpp::ll_error:
        mLogger->error("{}", event.message);
        break;
      case dpp::ll_critical:
      default:
        mLogger->critical("{}", event.message);
        break;
    }
  });

  bot.on_slashcommand(
      [this](auto event)
      {
        mLogger->debug("Event slashcommand: {}", event.command.get_command_name());
        if (event.command.get_command_name() == "ping")
        {
          event.reply("Pong!");
        }
      });

  bot.on_ready(
      [&bot, this](auto event)
      {
        if (dpp::run_once<struct register_bot_commands>())
        {
          mLogger->debug("Event on ready");
          bot.global_command_create(
              dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
      });

  bot.start(dpp::st_wait);
}

nb::Controller::~Controller()
{
  mLogger->debug(__func__);
}
