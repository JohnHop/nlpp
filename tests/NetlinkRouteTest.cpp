/**
 * @file NetlinkRouteTest.cpp
 * @brief Test the `Netlinkroute` class.
 */

#include "nlpp/NetlinkRoute.hpp"

#include <print>


int main(int argc, char* argv[])
{
  nlpp::NetlinkRoute nlroute;

  auto link = nlroute.get_kernel("wlp2s0"); // TODO: put you device
  std::println("{}", link.to_string());

  link = nlroute.get_kernel(nlpp::if_index_t{3}); // TODO: put you device
  std::println("{}", link.to_string());

  return EXIT_SUCCESS;
}