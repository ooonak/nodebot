#pragma once

#include <memory>
#include "dpp/dpp.h"

namespace nb {

class SlashCommandController {
 public:
  explicit SlashCommandController(std::shared_ptr<dpp::cluster> bot);

 private:
  std::shared_ptr<dpp::cluster> mBot;

};

} // namespace nb
