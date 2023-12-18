#pragma once

#include <memory>
#include <string>

#include "MQTTConfig.hpp"
#include "MessageQueue.hpp"
#include "spdlog/spdlog.h"

namespace ok
{

class MQTTClient;

class MQTTController
{
 public:
  explicit MQTTController(const std::shared_ptr<spdlog::logger>& logger, const ok::MQTTConfig config,
                          MessageQueue* ingressQueue);

  ~MQTTController();

  bool sendMessage(const ok::Message& msg) const;

 private:
  std::shared_ptr<spdlog::logger> logger_;
  std::unique_ptr<ok::MQTTClient> client_;
};

}  // namespace ok
