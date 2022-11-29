#include "WebHookController.hpp"

nb::WebHookController::WebHookController(std::shared_ptr<dpp::cluster> bot)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
}
