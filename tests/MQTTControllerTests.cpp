#include "gtest/gtest.h"
#include "MQTTController.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

TEST(MQTTController, ThrowIfNoLogger)
{
  auto logger = std::make_shared<spdlog::logger>("TestLogger", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  logger->set_level(spdlog::level::debug);
  spdlog::register_logger(logger);

  EXPECT_ANY_THROW(ok::MQTTController(nullptr));

  EXPECT_NO_THROW(ok::MQTTController(spdlog::get("TestLogger")));
}
