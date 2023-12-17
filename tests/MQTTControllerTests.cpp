#include "gtest/gtest.h"
#include "NodeBot/MQTTController.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

TEST(MQTTController, ThrowIfNoLogger)
{
  if (spdlog::get("MQTTControllerTests") == nullptr)
  {
    auto logger = std::make_shared<spdlog::logger>("MQTTControllerTests", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
  }

  EXPECT_ANY_THROW(ok::MQTTController(nullptr, ok::MQTTConfig{"NodeBotClient", "localhost", 1883, "nodebot"}, nullptr));

  EXPECT_NO_THROW(ok::MQTTController(spdlog::get("MQTTControllerTests"), ok::MQTTConfig{"NodeBotClient", "localhost", 1883, "nodebot"}, nullptr));
}
