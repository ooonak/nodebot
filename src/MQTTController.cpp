#include "NodeBot/MQTTController.hpp"

#include <mosquittopp.h>

#include <stdexcept>

#include "MQTTClient.hpp"

ok::MQTTController::MQTTController(const std::shared_ptr<spdlog::logger>& logger, const MQTTConfig config,
                                   IngressQueue* ingressQueue)
    : logger_{logger}
{
  if (logger_ == nullptr)
  {
    throw std::runtime_error("No logger (nullptr)");
  }

  /*
  int major = 0;
  int minor = 0;
  int revision = 0;
  const int version = mosquitto_lib_version(&major, &minor, &revision);
  logger_->info("Using mosquitto library v{}.{}.{} ({})", major, minor, revision, version);
  */

  mosqpp::lib_init();

  client_ = std::make_unique<MQTTClient>(logger_, std::move(config), ingressQueue);
  if (client_ != nullptr)
  {
    client_->loop_start();
  }
}

ok::MQTTController::~MQTTController()
{
  if (client_ != nullptr)
  {
    client_->disconnect();
    // This call will block until the network thread finishes
    client_->loop_stop(false);
  }

  mosqpp::lib_cleanup();

  logger_->info("Gracefull shutdown");
}
