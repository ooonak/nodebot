#pragma once

#include <memory>
#include "dpp/dpp.h"

namespace nb {

class ChannelController {
 public:
  explicit ChannelController(std::shared_ptr<dpp::cluster> bot);

 private:
  std::shared_ptr<dpp::cluster> mBot;

};

} // namespace nb
