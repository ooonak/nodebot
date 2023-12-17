#pragma once

#include "IngressMessage.hpp"

namespace ok
{

class IngressQueue
{
 public:
  virtual ~IngressQueue() = default;

  virtual void push(IngressMessage msg) = 0;

  virtual void waitAndPop(IngressMessage &msg) = 0;
};

}  // namespace ok
