#pragma once

#include <memory>
#include "dpp/dpp.h"

namespace nb {

class NodeController {
 public:
  explicit NodeController(std::shared_ptr<dpp::cluster> bot);

 private:
  std::shared_ptr<dpp::cluster> mBot;

};

} // namespace nb
