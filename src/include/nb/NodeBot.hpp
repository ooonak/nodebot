#pragma once

#include <memory>
#include <string>

namespace nb
{

class NodeBotImpl;

class NodeBot
{
 public:
  explicit NodeBot(const std::string &token);

  ~NodeBot();

  static std::string version();

 private:
  std::unique_ptr<NodeBotImpl> mImpl;
};

}  // namespace nb
