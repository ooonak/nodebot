#include "nb/NodeBot.hpp"

#include "Config.hpp"
#include "dpp/DppController.hpp"
#include "dpp/NodeQueues.hpp"
#include "nodebotconfig.hpp"

class nb::NodeBotImpl
{
 private:
  nb::Config mConfig;

  std::shared_ptr<nb::NodeQueues> mNodeQueues;
  std::unique_ptr<nb::DppController> mDppController;

 public:
  explicit NodeBotImpl(const std::string &filename, const std::shared_ptr<spdlog::logger> &logger)
  {
    const auto file = toml::parse(filename);
    mConfig = toml::find<nb::Config>(file, "config");
    mNodeQueues = std::make_shared<nb::NodeQueues>(mConfig, logger);
    mDppController = std::make_unique<nb::DppController>(mConfig, mNodeQueues, logger);
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

  uint64_t getHandle(const nb::NodeInfo &info)
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

nb::NodeBot::NodeBot(const std::string &filename, const std::shared_ptr<spdlog::logger> &logger)
    : mImpl{std::make_unique<nb::NodeBotImpl>(filename, logger)}
{
}

nb::NodeBot::~NodeBot() = default;

void nb::NodeBot::start() { return mImpl->start(); }

void nb::NodeBot::stop() { return mImpl->stop(); }

uint64_t nb::NodeBot::getHandle(const nb::NodeInfo &info) { return mImpl->getHandle(info); }

bool nb::NodeBot::updateNodeHandle(uint64_t id, const NodeInfo &info) { return mImpl->updateNodeHandle(id, info); }

bool nb::NodeBot::registerCommand(uint64_t id, std::string name, CmdCbT cb)
{
  return mImpl->registerCommand(id, name, cb);
}

bool nb::NodeBot::sendMessage(uint64_t id, std::string message) { return mImpl->sendMessage(id, message); }
