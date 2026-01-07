/**
 * @file NetlinkGenericTest.cpp
 * Test the `NetlinkGeneric` class.
 */


#include "nlpp/NetlinkGeneric.hpp"

#include <cstdlib>
#include <print>


int main(int argc, char* argv[])
{
  nlpp::NetlinkGeneric genl;

  // Test `get_interface()`, `get_list_interface()`
  auto interfaces = genl.get_list_interfaces();

  for(auto const& [if_index, dev_info]: interfaces)
  {
    if(genl.get_interface(dev_info.if_index) == dev_info) {
      std::println("{}\n", nlpp::to_string(dev_info));
    }
    else {
      return EXIT_FAILURE;
    }
  };

  // Test `get_phy()` and `get_list_phy()`
  auto phys = genl.get_list_phy();
  for(auto const& [phy_index, dev_cap]: phys) {
    auto phy = genl.get_phy(dev_cap.wiphy_index);
    std::println("{}\n", nlpp::to_string((phy)));
  }

  return EXIT_SUCCESS;
}