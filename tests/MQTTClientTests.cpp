#include "gtest/gtest.h"
#include "MQTTClient.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <mosquittopp.h>

TEST(MQTTClient, ThrowIfNoLogger)
{
  if (spdlog::get("MQTTClient") == nullptr)
  {
    auto logger = std::make_shared<spdlog::logger>("MQTTClient", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
  }

  mosqpp::lib_init();

  EXPECT_ANY_THROW(ok::MQTTClient(nullptr, "", "", 0));

  EXPECT_NO_THROW(ok::MQTTClient(spdlog::get("MQTTClient"), "MQTTClient", "localhost", 1883));

  mosqpp::lib_cleanup();
}
