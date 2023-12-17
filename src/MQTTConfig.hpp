#pragma once

#include <cstdint>
#include <string>

namespace ok
{

struct MQTTConfig
{
  const std::string id;
  const std::string host;
  const uint16_t port;
  const std::string base;
};

}  // namespace ok
