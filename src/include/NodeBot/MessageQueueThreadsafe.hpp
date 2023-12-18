#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "MessageQueue.hpp"
#include "utils/ThreadsafeQueue.hpp"

namespace ok
{

class MessageQueueThreadsafe : public MessageQueue
{
 public:
  void push(Message msg) override { queue_.push(msg); }

  void waitAndPop(Message &msg) override { queue_.waitAndPop(msg); }

 private:
  ThreadsafeQueue<Message> queue_;
};

}  // namespace ok
