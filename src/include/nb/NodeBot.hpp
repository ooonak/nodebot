#pragma once

#include <memory>
#include <string>
#include <vector>

#include "DataTypes.hpp"

namespace nb
{

class NodeBotImpl;

class NodeBot
{
 public:
  explicit NodeBot(const std::string &filename);

  ~NodeBot();

  void start();

  void stop();

  uint64_t getHandle(const NodeInfo &info);

 private:
  std::unique_ptr<NodeBotImpl> mImpl;
};

}  // namespace nb
