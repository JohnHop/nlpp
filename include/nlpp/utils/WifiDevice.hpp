#if !defined(NLPP_WIFIDEVICE_HPP)
#define NLPP_WIFIDEVICE_HPP


/** 
 * @file WifiDevice.hpp
 * Contains the `WifiDevice` definition.
 */


#include "nlpp/nlpp.hpp"
#include "nlpp/NetlinkRoute.hpp"
#include "nlpp/NetlinkGeneric.hpp"

#include <string>


namespace nlpp {


/** 
 * @brief Let you easily put wlan adapter to monitor mode and change channels.
 * @pre Device must have an index!
 */
class WifiDevice
{
public:

  /// @brief Construct a device issuing a request directly to the kernel.
  /// @param[in] if_name Wireless device name.
  /// @param[in] if_type Interface type to set.
  WifiDevice(std::string const&, nlpp::if_type_e = {});

// Getters / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /

  /// @brief Returns the device name retrieved from a `rtnl_link_t` object.
  [[nodiscard]] std::string name();

  /// @brief Get link status fom a `rtnl_link_t` obj and returns if it is UP.
  [[nodiscard]] bool is_up() noexcept;

  /// @brief Retrieve the interface type from a `rtnl_link_t` object.
  [[nodiscard]] nlpp::if_type_e type();

  /// @brief Retrieve the frequency.
  [[nodiscard]] std::optional<nlpp::frequency_t> frequency();

  /// @brief Retrieve the channel frequency.
  [[nodiscard]] std::optional<nlpp::channel_freq_t> channel();

  /// @brief Obtain the associated `dev_info_t` to this interface.
  [[nodiscard]] nlpp::dev_info_t dev_info();
  
  /// @brief Obtain the string representation.
  [[nodiscard]] std::string to_string();

// Setters / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /

  /// @brief Set interface up through using a `rtnl_link_t` object.
  void put_up();

  /// @brief Set interface down through using a `rtnl_link_t` object.
  void put_down();
  
  /// @brief Set interface type/mode.
  void set_type(nlpp::if_type_e);
  
  /// @brief Set device frequency.
  /// @param[in] freq Frequency to set.
  /// @throw // TODO when `freq` is not supported or invalid.
  void set_frequency(nlpp::frequency_t freq);

  /// @brief Set the device channel frequency.
  void set_channel_freq(nlpp::channel_freq_t chan);

private:

  nlpp::if_index_t     ifindex_;    // this device index
  nlpp::NetlinkRoute   nlroute_;    // connection to rtnl subsystem
  nlpp::NetlinkGeneric nlgeneric_;  // connection to genl subsystem
};


};  // end namespace nlpp


#endif // NLPP_WIFIDEVICE_HPP
