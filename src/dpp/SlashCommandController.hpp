#pragma once

#include <memory>

#include "InternalDataTypes.hpp"
#include "dpp/InternalDataTypes.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace ok
{

class SlashCommandController
{
 public:
  explicit SlashCommandController(std::shared_ptr<dpp::cluster> bot, ok::NodeHandlesT &nodes, const std::shared_ptr<spdlog::logger>& logger);

  void start(dpp::snowflake &guildId);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  ok::NodeHandlesT &mNodes;
  std::shared_ptr<spdlog::logger> mLogger;

  bool mInitialized{false};

  void onCommandCreated(const dpp::confirmation_callback_t &event);
  void onSlashCommand(const dpp::slashcommand_t &event);

  enum
  {
    NumberOfArgs = 3
  };
};

}  // namespace NodeBot
