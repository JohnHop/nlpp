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

  std::println("=== Test get_interface() and get_list_interface()");
  auto interfaces = genl.get_list_interfaces();

  for(auto const& [if_index, dev_info]: interfaces)
  {
    if(genl.get_interface(dev_info.if_index) == dev_info) {
      std::println("{}", nlpp::to_string(dev_info));
    }
    else {
      return EXIT_FAILURE;
    }
  };

  std::println("\n=== Test get_phy() and get_list_phy()");
  auto phys = genl.get_list_phys();
  for(auto const& [phy_index, dev_cap]: phys) {
    auto phy = genl.get_phy(dev_cap.wiphy_index);
    std::println("{}\n", nlpp::to_string((phy)));
  }

  std::println("\n=== Test set_if_type()");
  using namespace std::string_view_literals;

  std::string const if_name = "wlx7cf17e67ae25";  // TODO: put an already found wlan name

  auto const interfaceIt 
    = std::ranges::find_if(interfaces, [if_name](auto const& dev) {
        return dev.second.if_name == if_name; });

  auto const if_index = interfaceIt->second.if_index;

  genl.set_if_type(if_name, nlpp::if_type_e::monitor);
  std::println("{}", nlpp::to_string(genl.get_interface(if_index)));

  std::println("\n=== Test set_if_channel()");
  auto current_channel = nlpp::freq2chan(interfaceIt->second.wiphy_freq);
  auto const max_channel = nlpp::freq2chan(
    phys.at(interfaceIt->second.wiphy_index.get()).freqs.back());
  auto const new_channel 
    = nlpp::freq_chan_t{(current_channel.get() + 1) % max_channel.get()};

  genl.set_if_channel(if_name, new_channel);
  std::println("{}", nlpp::to_string(genl.get_interface(if_index)));

  return EXIT_SUCCESS;
}