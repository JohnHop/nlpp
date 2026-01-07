#if !defined(RTNLLINKT_HPP)
#define RTNLLINKT_HPP


/** 
 * @file rtnl_link_t.hpp
 * Contains the rtnl_link_t class definition.
 */

#include <netlink/route/link.h>

#include <string_view>
#include <optional>

#include "nlpp.hpp"


namespace nlpp {


/**
 * @brief Simple C++ wrapper around a `struct rtnl_link` with RAII.
 * 
 * This object represent a link device.
 * You can obtain this link object using a `nlcache_t` or directly from 
 * `rtnl_link_get_kernel()`.
 */
class rtnl_link_t
{
  struct rtnl_link* linkPtr_{}; // Underlying pointer

public:
  /// @brief Default constructortor. Allocate a empty link object.
  /// @throws `std::system_error` with `ENOMEM` error code.
  rtnl_link_t();

  /// @brief Construct a rtnl link from an existing `struct rtnl_link` object.
  /// @throws `std::logic_error` if `ptr` is `nullptr_t`.
  explicit rtnl_link_t(struct rtnl_link*);

  /// @brief Move constructor.
  rtnl_link_t(rtnl_link_t&&) noexcept;

  /// @brief Move assignment operator.
  /// @returns `*this`.
  rtnl_link_t& operator=(rtnl_link_t&&) noexcept;

  /// @brief Release and free the underlying resource.
  ~rtnl_link_t();

  /// @brief Returns a pointer to the managed object.
  /// @returns The underlying managed pointer.
  [[nodiscard]] struct rtnl_link* get_pointer() const noexcept;

  /// @brief Returns a string representation of the object.
  /// @returns The description of the rtnl link.
  [[nodiscard]] std::string to_string();

// libnl api -------------------------------------------------------------------

  /// @brief Returns link name or an empty string if no name exists.
  /// @returns The link name is the name assigned to a network interface.
  [[nodiscard]] std::string name() noexcept;

  /// @brief Returns link index (if specified for this link).
  /// @returns 
  [[nodiscard]] std::optional<if_index_t> get_index() noexcept;

  /// @brief Returns the operational state (also called link status).
  /// @returns Operational state of the link.
  [[nodiscard]] if_operstate_e operstate() noexcept;

  /// @brief Returns a bitset containing link flags.
  /// @returns Link flags.
  [[nodiscard]] if_flags_t flags() noexcept;

  /// @brief Set flags in order to enable various link features.
  void set_flags(if_flags_t) noexcept;

  /// @brief Unset flags in order to disable various link features.
  void unset_flags(if_flags_t) noexcept;

private:
  /// @brief Custom swap helper. Prevents recursive call of `std::swap()`.
  friend void swap(rtnl_link_t& lhs, rtnl_link_t& rhs) noexcept
  {
    std::swap(lhs.linkPtr_, rhs.linkPtr_);
  }
};


};


#endif // RTNLLINKT_HPP
