#if !defined(NLCACHE_HPP)
#define NLCACHE_HPP


/**
 * @file nlcache_t.hpp
 * Contains the `nlcache_t` class definition.
 */

#include "nlsocket_t.hpp"
#include "rtnl_link_t.hpp"

#include <optional>

#include <netlink/cache.h>


namespace nlpp
{


/**
 * @brief Simple C++ wrapper around a `struct nl_cache` with RAII.
 *
 * This object represent a list of links in the kernel.
 *
 * @see https://www.infradead.org/~tgr/libnl/doc/route.html#_get_list
 */
class nlcache_t
{
  struct nl_cache* cachePtr_{}; // underlying resource pointer

public:
  /// @brief Construct a cache object of all links from the kernel.
  /// @param[in] family Address family to use.
  nlcache_t(nlsocket_t&, int family = AF_UNSPEC);

  /// @brief Move ctor.
  nlcache_t(nlcache_t&&) noexcept;

  /// @brief Move assignment operator.
  /// @returns `*this`.
  nlcache_t& operator=(nlcache_t&&) noexcept;

  /// @brief Release the underlying resource.
  ~nlcache_t();

  /// @brief Return the pointer to the managed object.
  /// @returns The underlying managed pointer.
  [[nodiscard]] struct nl_cache* get_pointer() const noexcept;

// libnl API ------------------------------------------------------------------/

  /// @brief Retrieve a `rtnl_link_t`.
  /// @returns A `rtnl_link_t` object if found.
  [[nodiscard]] std::optional<rtnl_link_t> get_link(if_index_t);

  /// @brief Retrieve a `rtnl_link_t`.
  /// @param[in] ifname The interface name.
  /// @returns A `rtnl_link_t` object if found.
  [[nodiscard]] std::optional<rtnl_link_t> get_link(std::string_view ifname);

  /// @brief Retrieve the interface name from his index.
  /// @returns The interface name or a empty string.
  [[nodiscard]] std::string i2name(if_index_t);

  /// @brief Retrieve the interface index from his name through a `nlcache_t`.
  /// @param[in] ifname The interface name.
  /// @returns The interface index, if found.
  [[nodiscard]] std::optional<if_index_t> name2i(std::string_view ifname);

private:
  /// @brief Custom swap helper. Prevents recursive call of `std::swap()`.
  void swap(nlcache_t& lhs, nlcache_t& rhs) noexcept
  {
    std::swap(lhs.cachePtr_, rhs.cachePtr_);
  }  
};


};  // end `nlpp` namespace


#endif // NLCACHE_HPP
