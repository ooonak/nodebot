#include "NodeController.hpp"

nb::NodeController::NodeController(std::shared_ptr<dpp::cluster> bot)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
}
