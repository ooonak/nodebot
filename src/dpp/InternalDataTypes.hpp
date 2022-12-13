#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>

#include "nb/DataTypes.hpp"

namespace nb
{
struct NodeHandle
{
  uint64_t id;
  nb::NodeInfo info;
  std::chrono::system_clock::time_point created;
  std::chrono::system_clock::time_point lastActive;
  std::unordered_map<std::string, nb::CmdCbT> commandCallbacks;
};

using NodeHandlesT = std::vector<NodeHandle>;

using MessageT = std::pair<uint64_t, std::string>;
using MessageBufferT = std::deque<MessageT>;

}  // namespace nb
