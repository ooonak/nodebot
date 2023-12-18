#pragma once

#include "Message.hpp"

namespace ok
{

class MessageQueue
{
 public:
  virtual ~MessageQueue() = default;

  virtual void push(Message msg) = 0;

  virtual void waitAndPop(Message &msg) = 0;
};

}  // namespace ok
