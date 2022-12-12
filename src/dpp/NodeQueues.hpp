#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "dpp/InternalDataTypes.hpp"
#include "dpp/dpp.h"
#include "nb/DataTypes.hpp"
#include "spdlog/spdlog.h"

namespace nb
{

class NodeQueues
{
 public:
  explicit NodeQueues();

  ~NodeQueues() = default;

  uint64_t getNodeHandle(const nb::NodeInfo& info);

  bool updateNodeHandle(uint64_t id, const nb::NodeInfo& info);

  bool registerCommand(uint64_t id, std::string name, nb::CmdCbT cb);

  bool changes() const;

  NodeHandlesT nodes();

 private:
  std::shared_ptr<spdlog::logger> mLogger;
  mutable std::mutex mMutex;

  std::atomic<bool> mChanges{false};

  NodeHandlesT mNodeHandles;
};

}  // namespace nb
