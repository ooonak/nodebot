#include "NodeQueues.hpp"

#include <thread>

nb::NodeQueues::NodeQueues() : mLogger{spdlog::get("DPP")}
{
  mLogger->info("Starting {} in thread {}", __func__,
                std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

uint64_t nb::NodeQueues::getNodeHandle(const nb::NodeInfo& info)
{
  const auto now = std::chrono::system_clock::now();
  NodeHandle handle{ .id = 0, .info = info, .created = now, .lastActive = now };

  std::lock_guard<std::mutex> lock(mMutex);
  {
    handle.id = mNodeHandles.size() + 1;
    mNodeHandles.push_back(handle);
    mChanges = true;
  }

  mLogger->info("Registered new Node under id {}.", handle.id);

  return handle.id;
}

bool nb::NodeQueues::updateNodeHandle(uint64_t id, const nb::NodeInfo &info)
{
  std::lock_guard<std::mutex> lock(mMutex);
  {
    if (id > 0 && id <= mNodeHandles.size())
    {
      mNodeHandles.at(id-1).info = info;
      mNodeHandles.at(id-1).lastActive = std::chrono::system_clock::now();
      mChanges = true;
      return true;
    }
  }

  return false;
}

bool nb::NodeQueues::registerCommand(uint64_t id, std::string name, nb::CmdCbT cb)
{

}

bool nb::NodeQueues::changes() const
{
  return mChanges;
}

nb::NodeQueues::NodeHandlesT nb::NodeQueues::nodes()
{
  std::lock_guard<std::mutex> lock(mMutex);
  mChanges = false;
  return mNodeHandles;
}
