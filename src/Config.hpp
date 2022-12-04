#pragma once

#include <string>

#include "toml.hpp"

namespace nb
{

struct Config
{
  std::string token;
  std::string channelPrefix;
  int channelLifetimeInHours;
  int maxNodes;
  int updateFrequencySeconds;
};  // Config

}  // namespace nb

TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(nb::Config, token, channelPrefix,
                                       channelLifetimeInHours, maxNodes, updateFrequencySeconds)
