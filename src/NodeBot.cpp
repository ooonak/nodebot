#include "NodeBot/NodeBot.hpp"
#include "Config.hpp"
#include "dpp/DppController.hpp"
#include "dpp/NodeQueues.hpp"
#include "NodeBotConfig.h"

class ok::NodeBotImpl
{
 private:
  ok::Config mConfig;

  std::shared_ptr<ok::NodeQueues> mNodeQueues;
  std::unique_ptr<ok::DppController> mDppController;

 public:
  explicit NodeBotImpl(const std::string &filename, const std::shared_ptr<spdlog::logger> &logger)
  {
    const auto file = toml::parse(filename);
    mConfig = toml::find<ok::Config>(file, "config");
    mNodeQueues = std::make_shared<ok::NodeQueues>(mConfig, logger);
    mDppController = std::make_unique<ok::DppController>(mConfig, mNodeQueues, logger);
  }

  ~NodeBotImpl() = default;

  void start()
  {
    if (mDppController)
    {
      mDppController->start();
    }
  }

  void stop()
  {
    if (mDppController)
    {
      mDppController->stop();
    }
  }

  uint64_t getHandle(const ok::NodeInfo &info)
  {
    if (mNodeQueues != nullptr)
    {
      return mNodeQueues->getNodeHandle(info);
    }

    return 0;
  }

  bool updateNodeHandle(uint64_t id, const NodeInfo &info)
  {
    if (mNodeQueues != nullptr)
    {
      return mNodeQueues->updateNodeHandle(id, info);
    }

    return false;
  }

  bool registerCommand(uint64_t id, std::string name, CmdCbT cb)
  {
    if (mNodeQueues != nullptr)
    {
      return mNodeQueues->registerCommand(id, name, cb);
    }

    return false;
  }

  bool sendMessage(uint64_t id, std::string message)
  {
    if (mNodeQueues != nullptr)
    {
      return mNodeQueues->pushMessage(id, message);
    }

    return false;
  }
};

ok::NodeBot::NodeBot(const std::string &filename, const std::shared_ptr<spdlog::logger> &logger)
    : mImpl{std::make_unique<ok::NodeBotImpl>(filename, logger)}
{
}

ok::NodeBot::~NodeBot() = default;

void ok::NodeBot::start() { return mImpl->start(); }

void ok::NodeBot::stop() { return mImpl->stop(); }

uint64_t ok::NodeBot::getHandle(const ok::NodeInfo &info) { return mImpl->getHandle(info); }

bool ok::NodeBot::updateNodeHandle(uint64_t id, const NodeInfo &info) { return mImpl->updateNodeHandle(id, info); }

bool ok::NodeBot::registerCommand(uint64_t id, std::string name, CmdCbT cb)
{
  return mImpl->registerCommand(id, name, cb);
}

bool ok::NodeBot::sendMessage(uint64_t id, std::string message) { return mImpl->sendMessage(id, message); }
