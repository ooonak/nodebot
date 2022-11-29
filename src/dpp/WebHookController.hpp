#pragma once

#include <memory>

#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class WebHookController
{
 public:
  explicit WebHookController(std::shared_ptr<dpp::cluster> bot);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;
};

}  // namespace nb
