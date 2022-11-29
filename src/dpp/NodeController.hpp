#pragma once

#include <memory>

namespace nb {

class NodeController {
 public:
  explicit NodeController(std::shared_ptr<dpp::cluster bot> bot);

 private:
  std::shared_ptr<dpp::cluster bot> mBot;

};

} // namespace nb
