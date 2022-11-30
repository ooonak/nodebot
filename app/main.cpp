#include <cstdlib>
#include <iostream>

#include "nb/NodeBot.hpp"

int main()
{
  try
  {
    nb::NodeBot bot("/tmp/nodebot.conf");

    const auto jsonStr{ R"({ "name" : "Test", "info" : [ { "key" : "Key1", "value" : "Value 1" }, { "key" : "Key2", "value" : "Value 2" } ] })" };
    auto handle = bot.getHandle(jsonStr);
    if (handle != nullptr)
    {
      handle->msg("A message");
    }

  }
  catch (const std::exception& exc)
  {
    std::cerr << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
