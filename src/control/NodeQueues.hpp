#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "Config.hpp"
#include "control/InternalDataTypes.hpp"
#include "dpp/dpp.h"
#include "NodeBot/DataTypes.hpp"
#include "spdlog/spdlog.h"

namespace ok
{

class NodeQueues
{
 public:
  explicit NodeQueues(const ok::Config& config, const std::shared_ptr<spdlog::logger>& logger);

  ~NodeQueues() = default;

  uint64_t getNodeHandle(const ok::NodeInfo& info);

  bool updateNodeHandle(uint64_t id, const ok::NodeInfo& info);

  bool registerCommand(uint64_t id, std::string name, ok::CmdCbT cb);

  bool pushMessage(uint64_t id, std::string message);

  bool changes() const;

  NodeHandlesT nodes();

  bool messages() const;

  MessageT popMessage();

 private:
  const ok::Config& mConfig;
  std::shared_ptr<spdlog::logger> mLogger;
  mutable std::mutex mMutexNodeHandles;
  mutable std::mutex mMutexMessageBuffer;

  std::atomic<bool> mChanges{false};

  NodeHandlesT mNodeHandles;

  MessageBufferT mMessageBuffer;
};

}  // namespace NodeBot
