#pragma once

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

 private:
  std::shared_ptr<spdlog::logger> mLogger;
  std::mutex mMutex;

  struct NodeHandle
  {
    uint64_t id;
    nb::NodeInfo info;
  };

  using NodeHandlesT = std::vector<nb::NodeQueues::NodeHandle>;
  NodeHandlesT mNodeHandles;
};

}  // namespace nb
