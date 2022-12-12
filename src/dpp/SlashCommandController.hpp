#pragma once

#include <memory>

#include "dpp/InternalDataTypes.hpp"
#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class SlashCommandController
{
 public:
  explicit SlashCommandController(std::shared_ptr<dpp::cluster> bot, nb::NodeHandlesT &nodes);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  nb::NodeHandlesT & mNodes;
  std::shared_ptr<spdlog::logger> mLogger;
};

}  // namespace nb
