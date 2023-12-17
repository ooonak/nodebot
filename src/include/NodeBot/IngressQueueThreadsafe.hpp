#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "IngressQueue.hpp"
#include "utils/ThreadsafeQueue.hpp"

namespace ok
{

class IngressQueueThreadsafe : public IngressQueue
{
 public:
  void push(IngressMessage msg) override { queue_.push(msg); }

  void waitAndPop(IngressMessage &msg) override { queue_.waitAndPop(msg); }

 private:
  ThreadsafeQueue<IngressMessage> queue_;
};

}  // namespace ok
