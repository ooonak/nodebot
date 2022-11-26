#pragma once

#include <memory>

#include "Config.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class Controller
{
 public:
  explicit Controller(const nb::Config &config);

  ~Controller();

 private:
  const nb::Config &mConfig;
  std::shared_ptr<spdlog::logger> mLogger;

  std::unique_ptr<dpp::guild> mGuild;
  std::unique_ptr<dpp::channel> mChannel;
};

}  // namespace nb
