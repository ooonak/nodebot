#pragma once

#include <memory>
#include <string>

namespace nb
{

class NodeBotImpl;

class NodeBot
{
 public:
  explicit NodeBot(const std::string &filename);

  ~NodeBot();

 private:
  std::unique_ptr<NodeBotImpl> mImpl;

};

}  // namespace nb
