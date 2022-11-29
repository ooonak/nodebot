#pragma once

#include <memory>

namespace nb {

class ChannelController {
 public:
  explicit ChannelController(std::shared_ptr<dpp::cluster bot> bot);

 private:
  std::shared_ptr<dpp::cluster bot> mBot;

};

} // namespace nb
