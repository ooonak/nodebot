#pragma once

#include <atomic>
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
  explicit NodeQueues();

  ~NodeQueues() = default;

  uint64_t getNodeHandle(const nb::NodeInfo& info);

  bool changes() const;

  void getNodesInfo(std::vector<nb::NodeInfo> &nodesInfo);

 private:
  std::shared_ptr<spdlog::logger> mLogger;
  std::mutex mMutex;

  std::atomic<bool> mChanges{false};

  struct NodeHandle
  {
    uint64_t id;
    nb::NodeInfo info;
  };

  using NodeHandlesT = std::vector<nb::NodeQueues::NodeHandle>;
  NodeHandlesT mNodeHandles;
};

}  // namespace nb
