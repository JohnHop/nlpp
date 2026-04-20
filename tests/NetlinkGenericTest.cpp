/**
 * @file NetlinkGenericTest.cpp
 * Test the `NetlinkGeneric` class.
 */


#include "nlpp/NetlinkGeneric.hpp"
#include "nlpp/utils/WifiDevice.hpp"

#include <cstdlib>
#include <print>


/**
 * This function cover all public methods ✅
 * - NetlinkGeneric()
 * - get_interface()
 * - get_list_interfaces()
 * - get_phy()
 * - get_list_phys()
 * - set_if_type()
 * - set_if_channel() -> set_if_frequency()
 *
 * How to test:
 * 1) Plug your monitor-capable wlan dongle
 * 2) Execute `sudo ./NetlinkGenericTest <devname>`
 * 3) Analize the results
 */
int main(int argc, char* argv[])
{
  if(argc != 2) {
    std::println(stderr, "error: wrong usage. Specify a monitor-capable wlan");
    return EXIT_FAILURE;
  }

  nlpp::NetlinkGeneric genl;

  //* / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /

  /**
   * Get a list of all interfaces on this system with `get_list_interfaces()`.
   * Then, get each single interface with `get_interface()`.
   */

  std::println("=== Test `get_list_interface()` and `get_interface()` ===");

  auto interfaces = genl.get_list_interfaces(); // obtain all interfaces

  for(auto const& [if_index, _]: interfaces)
  {
    auto const dev_info = genl.get_interface(nlpp::if_index_t{if_index});
    std::println("{}", nlpp::to_string(dev_info));
  };

  //* / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / 

  /**
   * The a list of all phys on this system with `get_list_phys`.
   * Then, get each single phy with `get_phy()`.
   */

  std::println("\n=== Test `get_list_phys()` and `get_phy()` ===");
  
  auto const phys = genl.get_list_phys();

  for(auto const& [phy_index, dev_cap]: phys) 
  {
    auto phy = genl.get_phy(dev_cap.wiphy_index);
    std::println("{}\n", nlpp::to_string((phy)));
  }

  //* / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /

  /**
   * Test the `NetlinkGeneric::set_if_type()` call, trying to switch to monitor
   * mode.
   */

  std::string const if_name = argv[1];

  std::println(
    "\n=== Test `set_if_type()` with `{}` and `monitor` mode ===", 
    if_name);

  // find the device with name equal to `if_name`
  auto found_it = 
    std::ranges::find_if(
      interfaces, 
      [if_name](auto const& dev) { return dev.second.if_name == if_name; } );

  // if `if_name` not found
  if(found_it == std::cend(interfaces)) 
  {
    std::println(stderr, "error: {} not found", if_name);
    return EXIT_FAILURE;
  }

  auto const if_index = found_it->second.if_index;  // save the index from name

  nlpp::WifiDevice wlan{if_name};
  wlan.put_down();  // put the link down before set monitor-mode

  genl.set_if_type(if_name, nlpp::if_type_e::monitor);  // function to test

  wlan.put_up();  // put the link up to monitor-mode take effect

  // call `get_interface()` again to update the `dev_info_t` and print it
  found_it->second = genl.get_interface(if_index);
  std::println("{}", nlpp::to_string(found_it->second));

  //* / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /

  /**
   * Test the `NetlinkGeneric::set_if_channel()` function, trying to change the
   * current frequency channel.
   */
  std::println("\n=== Test `set_if_channel()` ===");

  if(!found_it->second.wiphy_freq.has_value())
  {
    std::println("error: unable to test set_if_channel() because no frequency");
    return EXIT_FAILURE;
  }

  auto const current_channel = 
    nlpp::freq2chan(found_it->second.wiphy_freq.value());

  auto const max_channel = 
    nlpp::freq2chan(phys.at(found_it->second.wiphy_index.get()).freqs.back());

  auto const new_channel = 
    nlpp::channel_freq_t{
      current_channel != max_channel ? (current_channel.get()+1) : 1};

  genl.set_if_channel(if_name, new_channel);

  std::println("{}", nlpp::to_string(genl.get_interface(if_index)));


  return EXIT_SUCCESS;
}