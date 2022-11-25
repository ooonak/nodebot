#include "nb/NodeBot.hpp"

#include <dpp/dpp.h>

#include <exception>
#include <iostream>

#include "nodebotconfig.hpp"

class nb::NodeBotImpl
{
 public:
  explicit NodeBotImpl(const std::string &token)
  {
    dpp::cluster bot(token);
    std::cout << "Using BOT_TOKEN: " << std::getenv("BOT_TOKEN") << std::endl;

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand(
        [](auto event)
        {
          std::cout << "Event slashcommand: "
                    << event.command.get_command_name() << std::endl;
          if (event.command.get_command_name() == "ping")
          {
            event.reply("Pong!");
          }
        });

    bot.on_ready(
        [&bot](auto event)
        {
          if (dpp::run_once<struct register_bot_commands>())
          {
            std::cout << "Event on ready" << std::endl;
            bot.global_command_create(
                dpp::slashcommand("ping", "Ping pong!", bot.me.id));
          }
        });

    bot.start(dpp::st_wait);
  }

  ~NodeBotImpl() = default;
};

nb::NodeBot::NodeBot(const std::string &token)
    : mImpl{std::make_unique<nb::NodeBotImpl>(token)}
{
}

std::string nb::NodeBot::version()
{
  return "NodeBot " + std::to_string(NodeBot_VERSION_MAJOR) + "." +
         std::to_string(NodeBot_VERSION_MINOR) + "." +
         std::to_string(NodeBot_VERSION_PATCH);
}

nb::NodeBot::~NodeBot() = default;
