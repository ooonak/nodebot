#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "IngressQueue.hpp"
#include "NodeBot/IngressQueueThreadsafe.hpp"
#include "NodeBot/MQTTController.hpp"
#include "NodeBot/NodeBot.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

void setupLogging(std::vector<spdlog::sink_ptr> &sinks)
{
  try
  {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    sinks.push_back(console_sink);

    auto mqttLogger = std::make_shared<spdlog::logger>("MQTT", std::begin(sinks), std::end(sinks));
    mqttLogger->set_level(spdlog::level::debug);
    spdlog::register_logger(mqttLogger);

    auto logger = std::make_shared<spdlog::logger>("DiscordBot", std::begin(sinks), std::end(sinks));
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
  }
  catch (const spdlog::spdlog_ex &ex)
  {
    std::cout << "Log initialization failed: " << ex.what() << std::endl;
  }
}

int main()
{
  try
  {
    std::vector<spdlog::sink_ptr> sinks;
    setupLogging(sinks);

    ok::IngressQueueThreadsafe queue;

    auto mqtt =
        ok::MQTTController(spdlog::get("MQTT"), ok::MQTTConfig{"NodeBotClient", "localhost", 1883, "nodebot"}, &queue);

    ok::IngressMessage msg{};
    while (1)
    {
      queue.waitAndPop(msg);
      spdlog::get("MQTT")->info(toString(msg));
    }

    /*
    ok::NodeBot bot("/tmp/nodebot.conf", spdlog::get("DiscordBot"));
    auto t1 = std::thread([&bot]() { bot.start(); });

    std::this_thread::sleep_for(std::chrono::seconds(2));

    ok::NodeInfo info{};
    info.name = "X-Wing #1";
    info.description = "Rebel T-65B Red Squadron.";
    info.details.push_back({"Direction", "To infinity"});
    info.details.push_back({"Speed", "Warp"});

    uint64_t handle1 = bot.getHandle(info);
    std::cout << "Got handle " << handle1 << std::endl;

    info.name = "TIE Fighter #2";
    info.description = "Galactic Empire Twin Ion Engine fighter.";
    info.details.clear();
    info.details.push_back({"Direction", "To hell"});
    info.details.push_back({"Speed", "Fast"});

    uint64_t handle2 = bot.getHandle(info);
    std::cout << "Got handle " << handle2 << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(10));
    info.name = "TIE Fighter #2";
    info.description = "Galactic Empire Twin Ion Engine fighter.";
    info.details.clear();
    info.details.push_back({"Direction", "Unknown"});
    info.details.push_back({"Speed", "Stopped"});
    bot.updateNodeHandle(handle2, info);

    auto pingCb = [&](const std::vector<std::string> &args) -> void
    {
      std::cout << "Ping command called, with arguments ";
      for (const auto &arg : args)
      {
        std::cout << arg << ", ";
      }
      std::cout << std::endl;

      bot.sendMessage(handle2, "Message send from within command");
    };

    if (!bot.registerCommand(handle2, "ping", pingCb))
    {
      std::cerr << "Could not register command" << std::endl;
    }

    bot.sendMessage(handle1, "A tests message :smile:");
    bot.sendMessage(handle2, "Another tests message :smile:");

    std::this_thread::sleep_for(std::chrono::seconds(180));
    bot.stop();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    t1.join();
    */
  }
  catch (const std::exception &exc)
  {
    std::cerr << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
