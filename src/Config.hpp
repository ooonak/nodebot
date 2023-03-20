#pragma once

#include <string>

#include "toml.hpp"

namespace nb
{

struct Config
{
  // Discord bot token
  std::string token;
  // Used for naming category
  std::string realm;
  // Used for naming channel under category, prepended with time of creation.
  std::string subRealm;
  // On bot start, removes all channels with creation time older than this.
  int channelLifetimeInHours;
  // How often to update node information.
  int updateFrequencySeconds;
  // Maximmu number og simultanious nodes.
  int maxNodes;
  // No more than this number of messages in queue.
  int maxMessagesInQueue;
};

}  // namespace nb

TOML11_DEFINE_CONVERSION_NON_INTRUSIVE(nb::Config, token, realm, subRealm, channelLifetimeInHours,
                                       updateFrequencySeconds, maxNodes, maxMessagesInQueue)
