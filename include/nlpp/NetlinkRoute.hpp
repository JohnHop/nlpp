#if !defined(NETLINKROUTE_HPP)
#define NETLINKROUTE_HPP


/**
 * @file NetlinkRoute.hpp
 * Contains the `NetlinkRoute` class definition.
 */

#include "nlpp.hpp"
#include "nlsocket_t.hpp"
#include "rtnl_link_t.hpp"


namespace nlpp
{


/**
 * @brief Utility used to obtain link object (`rtnl_link_t`) and change link
 *        status (es. `up` o `down` flag).
 */
class NetlinkRoute
{
  nlsocket_t socket_; // to connect to routing subsystem

public:
  /// @brief Connect to the Netlink Route subsystem.
  NetlinkRoute();

// libnl API

  /// @brief Obtain a link object representing a device from his name.
  /// @param[in] if_identifier Interface identifier.
  /// @returns A ``rtnl_link_t` link object  representing the device.
  /// @throws `std::runtime_error` when `rtnl_link_get_kernel()` fails.
  [[nodiscard]] rtnl_link_t get_link(if_identifier_t if_identifier);
  
  /// @brief Change a link object.
  /// @param[inout] origin Actual link to change.
  /// @param[in] change Link object containing the changes.
  /// @param[in] flags Optional flags.
  /// @throws `std::runtime_error` when `rtnl_link_change()` call fails.
  void link_change(rtnl_link_t& origin, rtnl_link_t& change, int flags = 0);

  // TODO: wrap rtnl_link_get_kernel()
  // rtnl_link_t get_kernel(nlsocket_t&, std::string const&)
  // rtnl_link_t get_kernel(nlsocket_t&, if_index_t);
};


};  // end namespace `nlpp`


#endif // NETLINKROUTE_HPP
