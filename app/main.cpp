#include <cstdlib>
#include <iostream>

#include "nb/NodeBot.hpp"

int main()
{
  std::cout << nb::NodeBot::version() << std::endl;

  try
  {
    const char* token = std::getenv("BOT_TOKEN");
    if (token == nullptr)
    {
      throw(std::invalid_argument("env variable BOT_TOKEN not set?"));
    }

    nb::NodeBot bot(token);
  }
  catch (const std::exception& exc)
  {
    std::cerr << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
