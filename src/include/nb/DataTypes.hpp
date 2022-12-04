#pragma once

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
};

}  // namespace nb
