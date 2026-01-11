#if !defined(NETLINKGENERIC_HPP)
#define NETLINKGENERIC_HPP


/**
 * @file NetlinkGeneric.hpp
 * Contains the `NetlinkGeneric` class definition.
 */

#include "nlpp.hpp"
#include "nlsocket_t.hpp"

#include <linux/nl80211.h>
#include <netlink/genl/ctrl.h>
#include <netlink/genl/genl.h>

#include <map>


namespace nlpp {


/**
 * @brief Utility whose API is mapped to some `iw` commands.
 * 
 * @details
 * Mapping function details:
 * - `get_interface()` -> `iw dev <devname> info`
 * - `get_list_interfaces()` -> `iw dev`
 * - `set_type()` -> `iw dev <devname> set type <type>`
 * - `set_channel()` -> `iw dev <devname> set channel <channel>`
 */
class NetlinkGeneric
{
  nlsocket_t socket_; // used to connect to genl service
  int nl80211_id;

public:
  /// @brief Default ctor. Connect to Netlink Generic subsystem.
  /// @throw `std::system_error` when `genl_ctrl_resolve()` call fails.
  NetlinkGeneric();

// libnl API ------------------------------------------------------------------/

  /// @brief Obtain information for a device.
  /// @param[in] if_index Interface index.
  /// @throws `std::system_error` when `if_index_t` is invalid.
  /// @returns A `dev_info_t` oebject about a device.
  /// @pre `if_index` must be a valid device index.
  /// @note This method corresponds to `iw dev <devname> info` command.
  [[nodiscard]] dev_info_t get_interface(if_index_t if_index);
  
  /// @brief Obtain a map of all devices info.
  /// @returns A `dev_info_t` map where key is the device index .
  /// @note This method correspond to `iw dev` command.
  [[nodiscard]] std::map<uint32_t,dev_info_t> get_list_interfaces();
   
  /// @brief Get capabilities for the specified wireless device.
  /// @param[in] phy_index Physical device index.
  /// @returns The device capability associated with a phisical index.
  /// @note This method corresponds to `iw phy <phyname> info`.
  [[nodiscard]] dev_capability_t get_phy(wiphy_index_t phy_index);


  /// @brief Get the list phy object.
  /// @return std::map<uint32_t,dev_capability_t> 
  [[nodiscard]] std::map<uint32_t,dev_capability_t> get_list_phys();
  
  /// @brief Change the interface type.
  /// @param[in] ifname Interface name.
  /// @param[in] type Interface type/mode to set.
  /// @pre Link must be put down (otherwise it throws resource busy).
  /// @note This method corresponds to `iw dev <devname> set type <type>`.
  void set_if_type(std::string const& ifname, if_type_e type);
  
  /// @brief Set the frequency.
  /// @param[in] ifname Interface name.
  /// @param[in] freq Frequency to set.
  /// @pre Link must be in monitor mode and up (oyherwise throws resource busy).
  /// @note This method corresponds to `iw dev <devname> set freq <freq>`.
  void set_if_frequency(std::string const& ifname, frequency_t freq);

  /// @brief Set the channel frequency.
  /// @param[in] ifname Interface name.
  /// @param[in] chan Channel frequency to set.
  /// @pre Link must be in monitor mode and up (oyherwise throws resource busy).
  /// @note This method corresponds to `iw dev <devname> set channel <channel>`.
  void set_if_channel(std::string const& ifname, channel_freq_t freq);

private:

  /// @brief Send a netlink message.
  /// @param[in] msg Netlink message.
  /// @param[in] fun Optional callback function.
  /// @param[in] arg Optional callback function parameter.
  /// @note You can address commands to a device only through his index.
  void send_msg(nlmsg_t const& msg, nl_recvmsg_msg_cb_t = {}, void* = {});

// Commands handlers callbacks

  /// @brief Callback to parse a `NL80211_CMD_GET_INTERFACE` response.
  static int get_interface_handler(struct nl_msg* msg, void* arg) noexcept;

  /// @brief Callback to parse a `NL80211_CMD_GET_PROTOCOL_FEATURES` response.
  static int get_feature_handler(struct nl_msg* msg, void* arg) noexcept;
  
  /// @brief Callback to parse a `NL80211_CMD_GET_WIPHY` response.
  static int get_phy_handler(struct nl_msg* msg, void* arg) noexcept;
};


};  // end namespace nlpp


#endif // NETLINKGENERIC_HPP
