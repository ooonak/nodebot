#include "NodeQueues.hpp"
#include <thread>

ok::NodeQueues::NodeQueues(const ok::Config& config, const std::shared_ptr<spdlog::logger>& logger)
    : mConfig{config}, mLogger{logger}
{
  if (mLogger == nullptr)
  {
    throw std::invalid_argument("Logger is nullptr");
  }

  mLogger->info("Starting {} in thread {}", __func__, std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

uint64_t ok::NodeQueues::getNodeHandle(const ok::NodeInfo& info)
{
  const auto now = std::chrono::system_clock::now();
  NodeHandle handle{.id = 0, .info = info, .created = now, .lastActive = now, .commandCallbacks = {} };

  const std::lock_guard<std::mutex> lock(mMutexNodeHandles);
  {
    handle.id = mNodeHandles.size() + 1;
    mNodeHandles.push_back(handle);
    mChanges = true;
  }

  mLogger->debug("Registered new Node under id {}.", handle.id);

  return handle.id;
}

bool ok::NodeQueues::updateNodeHandle(uint64_t id, const ok::NodeInfo& info)
{
  const std::lock_guard<std::mutex> lock(mMutexNodeHandles);
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

bool ok::NodeQueues::registerCommand(uint64_t id, std::string name, ok::CmdCbT cb)
{
  const std::lock_guard<std::mutex> lock(mMutexNodeHandles);
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

bool ok::NodeQueues::changes() const { return mChanges; }

ok::NodeHandlesT ok::NodeQueues::nodes()
{
  const std::lock_guard<std::mutex> lock(mMutexNodeHandles);
  mChanges = false;
  return mNodeHandles;
}

bool ok::NodeQueues::pushMessage(uint64_t id, std::string message)
{
  const std::lock_guard<std::mutex> lock(mMutexMessageBuffer);
  {
    if (mMessageBuffer.size() <= static_cast<size_t>(mConfig.maxMessagesInQueue))
    {
      mMessageBuffer.push_back({id, message});
      return true;
    }
  }

  return false;
}

bool ok::NodeQueues::messages() const
{
  const std::lock_guard<std::mutex> lock(mMutexMessageBuffer);
  return !mMessageBuffer.empty();
}

ok::MessageT ok::NodeQueues::popMessage()
{
  const std::lock_guard<std::mutex> lock(mMutexMessageBuffer);
  {
    if (!mMessageBuffer.empty())
    {
      auto msg = mMessageBuffer.front();
      mMessageBuffer.pop_front();
      return msg;
    }
  }
  return MessageT{};
}
