#include "gtest/gtest.h"
#include "NodeBot/MQTTController.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

TEST(MQTTController, ThrowIfNoLogger)
{
  if (spdlog::get("MQTTController") == nullptr)
  {
    auto logger = std::make_shared<spdlog::logger>("MQTTController", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
  }

  EXPECT_ANY_THROW(ok::MQTTController(nullptr, "NodeBot", "localhost", 1883));

  EXPECT_NO_THROW(ok::MQTTController(spdlog::get("MQTTController"), "NodeBot", "localhost", 1883));
}
