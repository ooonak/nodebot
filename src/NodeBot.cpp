#include "nb/NodeBot.hpp"
#include "dpp/DppController.hpp"
#include "Config.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "nodebotconfig.hpp"

class nb::NodeBotImpl
{
 private:
  nb::Config mConfig;
  std::unique_ptr<nb::DppController> mDppController;

  void setupLogging()
  {
    spdlog::init_thread_pool(8192, 2);
    std::vector<spdlog::sink_ptr> sinks;
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    sinks.push_back(stdout_sink);

    auto dppLogger = std::make_shared<spdlog::async_logger>("DPP", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    dppLogger->set_level(spdlog::level::level_enum::debug);
    spdlog::register_logger(dppLogger);

    auto l = spdlog::get("DPP");
    if (l != nullptr)
    {
      l->info("About to initialize NodeBot {}.{}.{}", NodeBot_VERSION_MAJOR, NodeBot_VERSION_MINOR, NodeBot_VERSION_PATCH);
    }
  }

 public:
  explicit NodeBotImpl(const std::string &filename)
  {
    const auto file = toml::parse(filename);
    mConfig = toml::find<nb::Config>(file, "config");

    setupLogging();

    mDppController = std::make_unique<nb::DppController>(mConfig);
  }

  ~NodeBotImpl() = default;

  std::shared_ptr<nb::NodeHandle> getHandle(const std::string &jsonStr)
  {
    if (mDppController != nullptr)
    {
      auto logger = spdlog::get("DPP");
      if (logger != nullptr)
      {
        logger->debug("{} {}", __PRETTY_FUNCTION__, jsonStr);
      }
    }

    return nullptr;
  }
};

nb::NodeBot::NodeBot(const std::string &filename)
    : mImpl{std::make_unique<nb::NodeBotImpl>(filename)}
{
}

nb::NodeBot::~NodeBot() = default;

std::shared_ptr<nb::NodeHandle> nb::NodeBot::getHandle(const std::string &jsonStr)
{
  return mImpl->getHandle(jsonStr);
}
