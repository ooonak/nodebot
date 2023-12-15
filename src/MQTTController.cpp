#include "MQTTController.hpp"
#include <mosquittopp.h>
#include <stdexcept>

ok::MQTTController::MQTTController(const std::shared_ptr<spdlog::logger> &logger)
 : logger_{logger}
 {
    if (logger_ == nullptr)
    {
        throw std::runtime_error("No logger (nullptr)");
    }
    
    int major = 0;
    int minor = 0;
    int revision = 0;
    const int version = mosquitto_lib_version(&major, &minor, &revision);
    logger_->info("Using mosquitto library v{}.{}.{} {}", major, minor, revision, version);

    mosqpp::lib_init();
 }

ok::MQTTController::~MQTTController()
{
    mosqpp::lib_cleanup();
}

void ok::MQTTController::operator()(ok::NodeActions *actions)
{
    if (actions != nullptr)
    {
        actions_ = actions;
    }
}
