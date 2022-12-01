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
  std::lock_guard<std::mutex> lock(mMutex);
  {
    handle.id = mNodeHandles.size() + 1;
    mNodeHandles.push_back(handle);
  }

  mLogger->info("Registered new Node under id {}.", handle.id);

  return handle.id;
}
