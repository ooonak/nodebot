#pragma once

#include "NodeActions.hpp"
#include <memory>
#include <string>
#include "spdlog/spdlog.h"

namespace ok {

class MQTTController {
public:
    explicit MQTTController(const std::shared_ptr<spdlog::logger> &logger);

    ~MQTTController();

private:
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace ok