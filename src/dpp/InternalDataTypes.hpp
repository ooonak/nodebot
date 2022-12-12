#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
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
} // namespace nb
