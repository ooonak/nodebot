#pragma once

#include <memory>

namespace nb {

class WebHookController {
 public:
  explicit WebHookController(std::shared_ptr<dpp::cluster bot> bot);

 private:
  std::shared_ptr<dpp::cluster bot> mBot;

};

} // namespace nb
