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

  /*
  bot.current_user_get_guilds([this](auto event){
    mLogger->debug("current_user_get_guilds ");
    if (!event.is_error()) {
      auto guilds = std::get<dpp::guild_map>(event.value);
      for (auto const &[key, value] : guilds)
      {
        mLogger->debug("guild {} {}", "key", "value");
      }
    }
  });
  */

  /*
  bot.guild_get(id, [&bot, this](auto& dpp::confirmation_callback_t &event){
    if (!event.is_error()) {
      auto guild = std::get<dpp::guild>(event.value);
      mLogger->debug("guild {} {} {}", guild.id, guild.name, guild.description);
    }
  });
  */

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
      [&bot, this](const dpp::ready_t event)
      {
        mLogger->debug("Event on ready 1");

        bot.current_user_get_guilds([&bot, this](auto event){
          mLogger->debug("current_user_get_guilds ");
          if (!event.is_error()) {
            auto guilds = std::get<dpp::guild_map>(event.value);
            for (auto const &[key, value] : guilds)
            {
              mLogger->debug("I'm a member of a guild called {} (id {}, created {}), {}", value.name, key, value.get_creation_time(), value.description);

              bot.channels_get(key, [&bot, this](auto event){
                if (!event.is_error()) {
                  auto channels = std::get<dpp::channel_map>(event.value);
                  for (auto const &[key, value] : channels)
                  {
                    mLogger->debug("Channel {} (id {})", value.name, key);
                  }
                                 }
                               });
            }
          }
        });

        mLogger->debug("Event on ready 2");

        if (dpp::run_once<struct register_bot_commands>())
        {
          mLogger->debug("bot.me.id {}, bot.me.name {}", bot.me.id, bot.me.username);
          //mLogger->debug("event.msg.channel_id {}", event.msg.channel_id);

          bot.global_command_create(
              dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }

        mLogger->debug("Event on ready 3");
      });

  bot.start(dpp::st_wait);
}

nb::Controller::~Controller()
{
  mLogger->debug(__func__);
}
