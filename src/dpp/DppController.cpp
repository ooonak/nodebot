#include "dpp/DppDppController.hpp"

#include <iostream>

nb::DppController::DppController(const nb::Config &config)
    : mConfig{config}, mLogger{spdlog::get("DPP")}
{
  mLogger->debug("{} {}", __func__, mConfig.token);

  dpp::cluster bot(mConfig.token);

  bot.on_log(
      [&bot, this](const dpp::log_t &event)
      {
        switch (event.severity)
        {
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
        mLogger->debug("Event slashcommand: {}",
                       event.command.get_command_name());
        if (event.command.get_command_name() == "ping")
        {
          event.reply("Pong!");
        }
      });

  bot.on_ready(
      [&bot, this](const dpp::ready_t event)
      {
        bot.current_user_get_guilds(
            [&bot, this](auto event)
            {
              mLogger->debug("current_user_get_guilds ");
              if (!event.is_error())
              {
                auto guilds = std::get<dpp::guild_map>(event.value);
                for (auto const &[key, value] : guilds)
                {
                  mGuild = std::make_unique<dpp::guild>(value);
                  bot.channels_get(
                      key,
                      [&bot, this](auto event)
                      {
                        if (!event.is_error())
                        {
                          auto channels =
                              std::get<dpp::channel_map>(event.value);
                          for (auto const &[key, value] : channels)
                          {
                            if (value.name == "nodes")
                            {
                              mChannel = std::make_unique<dpp::channel>(value);
                              mLogger->info(
                                  "I'm a Discord bot named '{}' ({}) connected "
                                  "to guild '{}' ({}), channel '{}' ({})",
                                  bot.me.username, bot.me.id, mGuild->name,
                                  mGuild->id, mChannel->name, mChannel->id);

                              bot.message_create(
                                  dpp::message(mChannel->id, "Hello :-)"));
                            }
                          }
                        }
                      });
                }
              }
            });

        if (dpp::run_once<struct register_bot_commands>())
        {
          // mLogger->debug("event.msg.channel_id {}", event.msg.channel_id);
          bot.global_command_create(
              dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
      });

  bot.start(dpp::st_wait);
}

nb::DppController::~DppController() { mLogger->debug(__func__); }
