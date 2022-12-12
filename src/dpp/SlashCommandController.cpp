#include "SlashCommandController.hpp"

nb::SlashCommandController::SlashCommandController(
    std::shared_ptr<dpp::cluster> bot, nb::NodeHandlesT &nodes)
    : mBot{bot}, mNodes{nodes}, mLogger{spdlog::get("DPP")}
{
  // TODO Remeber to look into dpp::utility::exec
}
