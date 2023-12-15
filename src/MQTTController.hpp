#pragma once

#include "NodeActions.hpp"
#include <memory>
#include <string>
#include "spdlog/spdlog.h"
#include "MQTTClient.hpp"

namespace ok {

class MQTTController {
public:
    explicit MQTTController(const std::shared_ptr<spdlog::logger> &logger);

    ~MQTTController();

    void operator()(ok::NodeActions *actions);

private:
    std::shared_ptr<spdlog::logger> logger_;
    ok::NodeActions *actions_{nullptr};
    std::unique_ptr<ok::MQTTClient> client_;
};

} // namespace ok