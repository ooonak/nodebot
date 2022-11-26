#include <cstdlib>
#include <iostream>

#include "nb/NodeBot.hpp"

int main()
{
  try
  {
    nb::NodeBot bot("/tmp/nodebot.conf");
  }
  catch (const std::exception& exc)
  {
    std::cerr << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
