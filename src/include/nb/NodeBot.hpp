#pragma once

#include <memory>
#include <string>
#include "NodeHandle.hpp"

namespace nb
{

class NodeBotImpl;

class NodeBot
{
 public:
  explicit NodeBot(const std::string &filename);

  ~NodeBot();

  std::shared_ptr<NodeHandle> getHandle(const std::string &jsonStr);

 private:
  std::unique_ptr<NodeBotImpl> mImpl;

};

}  // namespace nb
