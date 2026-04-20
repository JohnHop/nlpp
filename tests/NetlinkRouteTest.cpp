/**
 * @file NetlinkRouteTest.cpp
 * @brief Test the `Netlinkroute` class.
 */

#include "nlpp/NetlinkRoute.hpp"

#include <print>


int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    std::println("error: wrong usage. Specify a wlan name");
    return EXIT_FAILURE;
  }

  std::string const if_name = argv[1];

  nlpp::NetlinkRoute nlroute;

  auto link = nlroute.get_kernel(if_name);
  std::println("{}", link.to_string());

  auto const link_index = link.index();
  if(!link_index.has_value()) {
    std::println(
      "warning: interface {} has no index. Unable to test `get_kernel(if_index_t)`",
      if_name);
  }

  link = nlroute.get_kernel(link_index.value());
  std::println("{}", link.to_string());


  return EXIT_SUCCESS;
}