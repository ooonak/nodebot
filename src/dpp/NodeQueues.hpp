#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "dpp/dpp.h"
#include "nb/DataTypes.hpp"
#include "spdlog/spdlog.h"

namespace nb
{

class NodeQueues
{
 public:
  struct NodeHandle
  {
    uint64_t id;
    nb::NodeInfo info;
    std::chrono::system_clock::time_point created;
    std::chrono::system_clock::time_point lastActive;
  };

  using NodeHandlesT = std::vector<nb::NodeQueues::NodeHandle>;

  explicit NodeQueues();

  ~NodeQueues() = default;

  uint64_t getNodeHandle(const nb::NodeInfo& info);

  bool changes() const;

  //bool getNodes(NodeHandlesT &nodes) const;

  NodeHandlesT nodes();

 private:
  std::shared_ptr<spdlog::logger> mLogger;
  mutable std::mutex mMutex;

  std::atomic<bool> mChanges{false};

  NodeHandlesT mNodeHandles;
};

}  // namespace nb
