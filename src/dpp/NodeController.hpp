#pragma once

#include <memory>
#include <unordered_map>

#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace nb
{

class NodeController
{
 public:
  explicit NodeController(std::shared_ptr<dpp::cluster> bot);

  void update(uint64_t id, const std::string &jsonStr, dpp::snowflake channelId);

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;

  using MapValueT = std::pair<dpp::snowflake, std::string>;
  std::unordered_map<uint64_t, MapValueT> mNodes;

  void onMessageCreate(const dpp::confirmation_callback_t &event);

};

}  // namespace nb
