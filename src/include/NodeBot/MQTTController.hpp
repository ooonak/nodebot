#pragma once

#include <memory>
#include <string>
#include "spdlog/spdlog.h"
#include "MQTTConfig.hpp"

namespace ok {

class MQTTClient;
class IngressQueue;

class MQTTController {
public:
    explicit MQTTController(const std::shared_ptr<spdlog::logger> &logger, const ok::MQTTConfig config, IngressQueue* ingressQueue);

    ~MQTTController();

private:
    std::shared_ptr<spdlog::logger> logger_;
    std::unique_ptr<ok::MQTTClient> client_;
};

} // namespace ok
