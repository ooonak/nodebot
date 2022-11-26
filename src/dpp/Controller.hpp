#pragma once

#include "dpp/dpp.h"
#include "Config.hpp"
#include "spdlog/spdlog.h"

namespace nb {

class Controller {
 public:
  explicit Controller(const nb::Config &config);

  ~Controller();

private:
  const nb::Config & mConfig;
  std::shared_ptr<spdlog::logger> mLogger;
};

} // namespace nb
