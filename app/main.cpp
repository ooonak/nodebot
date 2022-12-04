#include <cstdlib>
#include <iostream>
#include <thread>

#include "nb/NodeBot.hpp"

int main()
{
  try
  {
    nb::NodeBot bot("/tmp/nodebot.conf");
    auto t1 = std::thread([&bot]() { bot.start(); });

    std::this_thread::sleep_for(std::chrono::seconds(5));

    nb::NodeInfo info{};
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

    std::this_thread::sleep_for(std::chrono::seconds(60));
    bot.stop();
    t1.join();
  }
  catch (const std::exception& exc)
  {
    std::cerr << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
