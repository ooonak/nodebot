#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ok
{

using CmdCbT = std::function<void(const std::vector<std::string>& args)>;

struct NodeInfo
{
  std::string name;
  std::string description;
  std::vector<std::pair<std::string, std::string>> details;
};

}  // namespace ok
