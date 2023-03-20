#include "NodeQueues.hpp"
#include <thread>

nb::NodeQueues::NodeQueues(const nb::Config& config, const std::shared_ptr<spdlog::logger>& logger)
    : mConfig{config}, mLogger{logger}
{
  if (mLogger == nullptr)
  {
    throw std::invalid_argument("Logger is nullptr");
  }

  mLogger->info("Starting {} in thread {}", __func__, std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

uint64_t nb::NodeQueues::getNodeHandle(const nb::NodeInfo& info)
{
  const auto now = std::chrono::system_clock::now();
  NodeHandle handle{.id = 0, .info = info, .created = now, .lastActive = now};

  std::lock_guard<std::mutex> lock(mMutexNodeHandles);
  {
    handle.id = mNodeHandles.size() + 1;
    mNodeHandles.push_back(handle);
    mChanges = true;
  }

  mLogger->debug("Registered new Node under id {}.", handle.id);

  return handle.id;
}

bool nb::NodeQueues::updateNodeHandle(uint64_t id, const nb::NodeInfo& info)
{
  std::lock_guard<std::mutex> lock(mMutexNodeHandles);
  {
    if (id > 0 && id <= mNodeHandles.size())
    {
      mNodeHandles.at(id - 1).info = info;
      mNodeHandles.at(id - 1).lastActive = std::chrono::system_clock::now();
      mChanges = true;
      return true;
    }
  }

  return false;
}

bool nb::NodeQueues::registerCommand(uint64_t id, std::string name, nb::CmdCbT cb)
{
  std::lock_guard<std::mutex> lock(mMutexNodeHandles);
  {
    if (id > 0 && id <= mNodeHandles.size())
    {
      if (mNodeHandles.at(id - 1).commandCallbacks.find(name) ==
          mNodeHandles.at(id - 1).commandCallbacks.end())
      {
        mNodeHandles.at(id - 1).commandCallbacks[name] = cb;
        mLogger->debug("Registered command callback '{}' for node with id {}.",
                       name, id);
        mChanges = true;
        return true;
      }
    }
  }

  return false;
}

bool nb::NodeQueues::changes() const { return mChanges; }

nb::NodeHandlesT nb::NodeQueues::nodes()
{
  std::lock_guard<std::mutex> lock(mMutexNodeHandles);
  mChanges = false;
  return mNodeHandles;
}

bool nb::NodeQueues::pushMessage(uint64_t id, std::string message)
{
  std::lock_guard<std::mutex> lock(mMutexMessageBuffer);
  {
    if (mMessageBuffer.size() <= mConfig.maxMessagesInQueue)
    {
      mMessageBuffer.push_back({id, message});
      return true;
    }
  }

  return false;
}

bool nb::NodeQueues::messages() const
{
  std::lock_guard<std::mutex> lock(mMutexMessageBuffer);
  return !mMessageBuffer.empty();
}

nb::MessageT nb::NodeQueues::popMessage()
{
  std::lock_guard<std::mutex> lock(mMutexMessageBuffer);
  {
    if (!mMessageBuffer.empty())
    {
      auto msg = mMessageBuffer.front();
      mMessageBuffer.pop_front();
      return msg;
    }
  }
}
