#include "gtest/gtest.h"
#include "MQTTClient.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

TEST(MQTTClient, ThrowIfNoLogger)
{
  if (spdlog::get("TestLogger") == nullptr)
  {
    auto logger = std::make_shared<spdlog::logger>("TestLogger", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
  }

  EXPECT_ANY_THROW(ok::MQTTClient(nullptr));

  EXPECT_NO_THROW(ok::MQTTClient(spdlog::get("TestLogger")));
}
