#pragma once

#include <memory>
#include "dpp/dpp.h"

namespace nb {

class WebHookController {
 public:
  explicit WebHookController(std::shared_ptr<dpp::cluster> bot);

 private:
  std::shared_ptr<dpp::cluster> mBot;

};

} // namespace nb
