#include "ChannelController.hpp"

nb::ChannelController::ChannelController(std::shared_ptr<dpp::cluster> bot)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
}
