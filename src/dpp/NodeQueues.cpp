#include "NodeQueues.hpp"

#include <thread>

nb::NodeQueues::NodeQueues() : mLogger{spdlog::get("DPP")}
{
  mLogger->info("Starting {} in thread {}", __func__,
                std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

uint64_t nb::NodeQueues::getNodeHandle(const nb::NodeInfo& info)
{
  NodeHandle handle{ .id = 0, .info = info, .created = std::chrono::system_clock::now() };

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

/*
bool nb::NodeQueues::getNodes(nb::NodeQueues::NodeHandlesT &nodes) const
{
  if (mChanges == false)
    return false;

  std::lock_guard<std::mutex> lock(mMutex);
  nodes = mNodeHandles;
  return true;
}
*/

nb::NodeQueues::NodeHandlesT nb::NodeQueues::nodes()
{
  std::lock_guard<std::mutex> lock(mMutex);
  mChanges = false;
  return mNodeHandles;
}
