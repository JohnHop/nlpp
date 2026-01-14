#if !defined(WIFIDEVICE_HPP)
#define WIFIDEVICE_HPP


/**
 * @file WifiDevice.hpp
 * Contains the `WifiDevice` definition.
 */

#include "nlpp/nlpp.hpp"
#include "nlpp/NetlinkRoute.hpp"
#include "nlpp/NetlinkGeneric.hpp"

#include <string>


/**
 * @brief Let you easily put wlan adapter to monitor mode and change channels.
 * @pre Device must have an index!
 */
class WifiDevice
{
  nlpp::if_index_t     ifindex_;    // device index
  nlpp::NetlinkRoute   nlroute_;    // connection to rtnl subsystem
  nlpp::NetlinkGeneric nlgeneric_;  // connection to genl subsystem

public:

  /// @brief Construct a device issuing a request directly to the kernel.
  /// @param[in] if_name Wireless device name.
  /// @param[in] if_type Interface type to set.
  WifiDevice(std::string const&, nlpp::if_type_e = {});

// Getters

  /// @brief Returns the device name retrieved from a `rtnl_link_t` object.
  [[nodiscard]] std::string get_name();

  /// @brief Get link status fom a `rtnl_link_t` obj and returns if it is UP.
  [[nodiscard]] bool is_up() noexcept;

  /// @brief Retrieve the interface type from a `rtnl_link_t` object and returns it.
  [[nodiscard]] nlpp::if_type_e get_type();

  /// @brief Retrieve the frequency.
  [[nodiscard]] std::optional<nlpp::frequency_t> get_frequency();
  
  /// @brief Obtain the string representation.
  [[nodiscard]] std::string to_string();

// Setters

  /// @brief Set interface up through using a `rtnl_link_t` object.
  void put_up();

  /// @brief Set interface down through using a `rtnl_link_t` object.
  void put_down();
  
  /// @brief Set interface type/mode.
  void set_type(nlpp::if_type_e);
  
  /// @brief Set device frequency.
  /// @param[in] freq Frequency to set.
  /// @throw TODO when `freq` is not supported or invalid.
  void set_frequency(nlpp::frequency_t freq);
};


#endif // WIFIDEVICE_HPP
