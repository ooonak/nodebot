#include "SlashCommandController.hpp"

nb::SlashCommandController::SlashCommandController(
    std::shared_ptr<dpp::cluster> bot)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
  // TODO Remeber to look into dpp::utility::exec
}
