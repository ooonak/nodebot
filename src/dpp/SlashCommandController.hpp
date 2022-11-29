#pragma once

#include <memory>

namespace nb {

class SlashCommandController {
 public:
  explicit SlashCommandController(std::shared_ptr<dpp::cluster bot> bot);

 private:
  std::shared_ptr<dpp::cluster bot> mBot;

};

} // namespace nb
