#pragma once

#include <chrono>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>
#include "NodeBot/DataTypes.hpp"

namespace ok
{
struct NodeHandle
{
  uint64_t id;
  ok::NodeInfo info;
  std::chrono::system_clock::time_point created;
  std::chrono::system_clock::time_point lastActive;
  std::unordered_map<std::string, ok::CmdCbT> commandCallbacks;
};

using NodeHandlesT = std::vector<NodeHandle>;

using MessageT = std::pair<uint64_t, std::string>;
using MessageBufferT = std::deque<MessageT>;

}  // namespace NodeBot
