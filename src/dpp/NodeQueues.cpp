#include "NodeQueues.hpp"

#include <thread>

nb::NodeQueues::NodeQueues() : mLogger{spdlog::get("DPP")}
{
  mLogger->info("Starting {} in thread {}", __func__,
                std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

uint64_t nb::NodeQueues::getNodeHandle(const nb::NodeInfo& info)
{
  NodeHandle handle{0, info};
  handle.info.created = std::chrono::system_clock::now();

  std::lock_guard<std::mutex> lock(mMutex);
  {
    handle.id = mNodeHandles.size() + 1;
    mNodeHandles.push_back(handle);
    mChanges = true;
  }

  mLogger->info("Registered new Node under id {}.", handle.id);

  return handle.id;
}

bool nb::NodeQueues::changes() const
{
  return mChanges;
}

void nb::NodeQueues::getNodesInfo(std::vector<nb::NodeInfo> &nodesInfo)
{
  if (mChanges == false)
  {
    return;
  }

  std::lock_guard<std::mutex> lock(mMutex);
  {
    for (const auto node : mNodeHandles)
    {
      nodesInfo.push_back(node.info);
    }
    mChanges = false;
  }
}
