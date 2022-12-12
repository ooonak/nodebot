#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace nb
{

using CmdCbT = std::function<void(const std::vector<std::string> & args)>;

struct NodeInfo
{
  std::string name;
  std::string description;
  std::vector<std::pair<std::string, std::string>> details;
};

}  // namespace nb
