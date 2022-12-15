#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "Config.hpp"
#include "dpp/InternalDataTypes.hpp"
#include "dpp/dpp.h"
#include "nb/DataTypes.hpp"
#include "spdlog/spdlog.h"

namespace nb
{

class NodeQueues
{
 public:
  explicit NodeQueues(const nb::Config& config);

  ~NodeQueues() = default;

  uint64_t getNodeHandle(const nb::NodeInfo& info);

  bool updateNodeHandle(uint64_t id, const nb::NodeInfo& info);

  bool registerCommand(uint64_t id, std::string name, nb::CmdCbT cb);

  bool pushMessage(uint64_t id, std::string message);

  bool changes() const;

  NodeHandlesT nodes();

  bool messages() const;

  MessageT popMessage();

 private:
  const nb::Config& mConfig;
  std::shared_ptr<spdlog::logger> mLogger;
  mutable std::mutex mMutexNodeHandles;
  mutable std::mutex mMutexMessageBuffer;

  std::atomic<bool> mChanges{false};

  NodeHandlesT mNodeHandles;

  MessageBufferT mMessageBuffer;
};

}  // namespace nb
