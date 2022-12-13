#include "SlashCommandController.hpp"

using namespace std::placeholders;

nb::SlashCommandController::SlashCommandController(
    std::shared_ptr<dpp::cluster> bot, nb::NodeHandlesT &nodes)
    : mBot{bot}, mNodes{nodes}, mLogger{spdlog::get("DPP")}
{
}

void nb::SlashCommandController::start(dpp::snowflake &guildId)
{
  if (!mInitialized)
  {
    mInitialized = true;

    mLogger->debug(__PRETTY_FUNCTION__);

    mBot->on_slashcommand(
        std::bind(&SlashCommandController::onSlashCommand, this, _1));

    dpp::slashcommand helpCommand("help", "Print help message", mBot->me.id);

    dpp::slashcommand cmdCommand("cmd", "Send a command", mBot->me.id);

    auto idOption = dpp::command_option(
        dpp::co_integer, "id",
        "The ID of the node as dislayed in it's embed message.", true);
    idOption.set_min_value(dpp::command_option_range{0});
    cmdCommand.add_option(idOption);

    auto cmdOption =
        dpp::command_option(dpp::co_string, "cmd", "Command to invoke.", true);
    cmdOption.set_max_length(dpp::command_option_range{256});
    cmdCommand.add_option(cmdOption);

    for (size_t i = 0; i < NumberOfArgs; ++i)
    {
      auto argOption = dpp::command_option(
          dpp::co_string, "arg" + std::to_string(i),
          "Optional argument " + std::to_string(i + 1) + "to command.", false);
      argOption.set_max_length(dpp::command_option_range{256});
      cmdCommand.add_option(argOption);
    }

    mBot->guild_command_create(
        helpCommand, guildId,
        std::bind(&SlashCommandController::onCommandCreated, this, _1));

    mBot->guild_command_create(
        cmdCommand, guildId,
        std::bind(&SlashCommandController::onCommandCreated, this, _1));
  }
}

void nb::SlashCommandController::onCommandCreated(
    const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    mLogger->info("Registered command");
  }
}

void nb::SlashCommandController::onSlashCommand(
    const dpp::slashcommand_t &event)
{
  const auto start = std::chrono::steady_clock::now();

  mLogger->debug("Slash command event:  {}.", event.command.get_command_name());

  if (event.command.get_command_name() == "ping")
  {
    event.reply(std::string("pong"));
  }
  else if (event.command.get_command_name() == "help")
  {
    std::ostringstream oss;
    oss << "Available nodes and their commands\n";

    for (const auto &node : mNodes)
    {
      oss << "Id: " << node.id << ", Name: " << node.info.name;

      if (!node.commandCallbacks.empty())
      {
        oss << ", Commands: ";
        for (const auto &[key, value] : node.commandCallbacks)
        {
          oss << key << " ";
        }
      }
      oss << "\n";
    }

    event.reply(oss.str());
  }
  else if (event.command.get_command_name() == "cmd")
  {
    // Non-optional parameters
    const auto id = std::get<int64_t>(event.get_parameter("id"));
    const auto cmdName = std::get<std::string>(event.get_parameter("cmd"));

    auto itVec = std::find_if(mNodes.begin(), mNodes.end(),
                              [&](const auto &node) { return node.id == id; });
    if (itVec != mNodes.end())
    {
      auto itUMap = (*itVec).commandCallbacks.find(cmdName);
      if (itUMap != (*itVec).commandCallbacks.end())
      {
        std::vector<std::string> args{};

        // Optional parameters
        for (size_t i = 0; i < NumberOfArgs; ++i)
        {
          auto val = event.get_parameter("arg" + std::to_string(i));
          if (std::holds_alternative<std::string>(val))
          {
            args.push_back(std::get<std::string>(val));
          }
        }

        itUMap->second(args);

        const auto end = std::chrono::steady_clock::now();
        // TODO One could look into dpp::utility::exec to execute command in
        // other thread.
        event.reply("Job done after " +
                    std::to_string(
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            end - start)
                            .count()) +
                    " ms.");
      }
      else
      {
        event.reply("Node " + std::to_string(id) +
                    ", has no callback registered for command '" + cmdName +
                    "'.");
      }
    }
    else
    {
      event.reply("Unknown node " + std::to_string(id) +
                  ", valid values are 0-" + std::to_string(mNodes.size()) +
                  ".");
    }
  }
}
