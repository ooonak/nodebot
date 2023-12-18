#include <mosquittopp.h>

#include "MQTTClient.hpp"
#include "gtest/gtest.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

TEST(MQTTClient, ThrowIfNoLogger)
{
  mosqpp::lib_init();

  EXPECT_ANY_THROW(ok::MQTTClient(nullptr, ok::MQTTConfig{"NodeBotClient", "localhost", 1883, "nodebot"}, nullptr));

  mosqpp::lib_cleanup();
}

TEST(MQTTClient, DontThrowIfLogger)
{
  if (spdlog::get("MQTTClientTests") == nullptr)
  {
    auto logger =
        std::make_shared<spdlog::logger>("MQTTClientTests", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
  }

  mosqpp::lib_init();

  EXPECT_NO_THROW(ok::MQTTClient(spdlog::get("MQTTClientTests"),
                                 ok::MQTTConfig{"NodeBotClient", "localhost", 1883, "nodebot"}, nullptr));

  mosqpp::lib_cleanup();
}

TEST(MQTTClient, containsNonAlpha)
{
  EXPECT_TRUE(ok::MQTTClient::containsNonAlpha("æ"));
  EXPECT_TRUE(ok::MQTTClient::containsNonAlpha("."));
  EXPECT_TRUE(ok::MQTTClient::containsNonAlpha("A"));
  EXPECT_FALSE(ok::MQTTClient::containsNonAlpha("abcdefghijklmnopqrstuvwxyz"));
}

TEST(MQTTClient, parseOk)
{
  const std::string topic = "/nodebot/group/1/connect";
  const auto msg = ok::MQTTClient::parse(topic, nullptr);
  ASSERT_TRUE(msg.has_value());
  EXPECT_EQ((*msg).id, std::uint64_t{1});
  EXPECT_EQ((*msg).group, "group");
  EXPECT_EQ((*msg).action, ok::Message::ActionT::Connect);
}

TEST(MQTTClient, parseToLong)
{
  const std::string topic = "/nodebot/group/subgroup/1/connect";
  const auto msg = ok::MQTTClient::parse(topic, nullptr);
  EXPECT_FALSE(msg.has_value());
}

TEST(MQTTClient, parseToShort)
{
  const std::string topic = "/nodebot/1/connect";
  const auto msg = ok::MQTTClient::parse(topic, nullptr);
  EXPECT_FALSE(msg.has_value());
}

TEST(MQTTClient, parseBadInput)
{
  const std::string topic1 = "/nodebot/group/UUID/connect";
  const auto msg1 = ok::MQTTClient::parse(topic1, nullptr);
  EXPECT_FALSE(msg1.has_value());

  const std::string topic2 = "/nodebot/group/42/cONNECT";
  const auto msg2 = ok::MQTTClient::parse(topic2, nullptr);
  EXPECT_FALSE(msg2.has_value());

  const std::string topic3 = "nodebot//2/disconnect";
  const auto msg3 = ok::MQTTClient::parse(topic3, nullptr);
  EXPECT_FALSE(msg3.has_value());
}
