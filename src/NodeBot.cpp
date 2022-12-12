#include "nb/NodeBot.hpp"

#include "Config.hpp"
#include "dpp/DppController.hpp"
#include "dpp/NodeQueues.hpp"
#include "nodebotconfig.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

class nb::NodeBotImpl
{
 private:
  nb::Config mConfig;

  std::shared_ptr<nb::NodeQueues> mNodeQueues;
  std::unique_ptr<nb::DppController> mDppController;

  void setupLogging()
  {
    spdlog::init_thread_pool(8192, 2);
    std::vector<spdlog::sink_ptr> sinks;
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sinks.push_back(stdout_sink);

    auto dppLogger = std::make_shared<spdlog::async_logger>(
        "DPP", sinks.begin(), sinks.end(), spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);
    dppLogger->set_level(spdlog::level::level_enum::debug);
    spdlog::register_logger(dppLogger);

    auto l = spdlog::get("DPP");
    if (l != nullptr)
    {
      l->info("About to initialize NodeBot {}.{}.{}", NodeBot_VERSION_MAJOR,
              NodeBot_VERSION_MINOR, NodeBot_VERSION_PATCH);
    }
  }

 public:
  explicit NodeBotImpl(const std::string &filename)
  {
    const auto file = toml::parse(filename);
    mConfig = toml::find<nb::Config>(file, "config");

    setupLogging();

    mNodeQueues = std::make_shared<nb::NodeQueues>();
    mDppController = std::make_unique<nb::DppController>(mConfig, mNodeQueues);
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
    /*
    if (mNodeQueues != nullptr)
    {
      return mNodeQueues->registerCommand(id, name, cb);
    }
    */

    return false;
  }
};

nb::NodeBot::NodeBot(const std::string &filename)
    : mImpl{std::make_unique<nb::NodeBotImpl>(filename)}
{
}

nb::NodeBot::~NodeBot() = default;

void nb::NodeBot::start() { return mImpl->start(); }

void nb::NodeBot::stop() { return mImpl->stop(); }

uint64_t nb::NodeBot::getHandle(const nb::NodeInfo &info)
{
  return mImpl->getHandle(info);
}

bool nb::NodeBot::updateNodeHandle(uint64_t id, const NodeInfo &info)
{
  return mImpl->updateNodeHandle(id, info);
}

bool nb::NodeBot::registerCommand(uint64_t id, std::string name, CmdCbT cb)
{
  return mImpl->registerCommand(id, name, cb);
}

bool nb::NodeBot::sendMessage(uint64_t id, std::string message)
{
  return mImpl->sendMessage(id, message);
}
