#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
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

  bool updateNodeHandle(uint64_t id, const NodeInfo &info);

  bool registerCommand(uint64_t id, std::string name, CmdCbT cb);

 private:
  std::unique_ptr<NodeBotImpl> mImpl;
};

}  // namespace nb
