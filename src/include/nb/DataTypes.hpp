#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace nb
{

struct NodeInfo
{
  std::string name;
  std::string description;
  std::vector<std::pair<std::string, std::string>> details;

  std::chrono::system_clock::time_point created;
  std::chrono::system_clock::time_point lastActive;
};

}  // namespace nb
