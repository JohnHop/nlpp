#if !defined(NLMSGT_HPP)
#define NLMSGT_HPP


/** 
 * @file nlmsg_t.hpp
 * Contains the `nlmsg_t` class definition.
 */


#include "nlattr_t.hpp"

#include <linux/nl80211.h>
#include <netlink/msg.h>

#include <concepts>
#include <utility>


namespace nlpp {


/**
 * @brief Simple C++ wrapper around a `struct nl_msg` with RAII resource 
 *        management.
 */
class nlmsg_t
{
  struct nl_msg* msgPtr_{}; // underlying pointer

public:
  /// @brief Create a empty netlink message.
  /// @throws `std::system_error` with `ENOMEM` errno code.
  nlmsg_t();
  
  /// @brief Quickly create a netlink message and put a genl header.
  /// @param[in] family Netlink family to use.
  /// @param[in] cmd Command to use.
  /// @param[in] flags Flags to append.
  nlmsg_t(int family, nl80211_commands cmd, int flags=0);

  /// @brief Move ctor.
  nlmsg_t(nlmsg_t&&) noexcept;

  /// @brief Move assignment operator.
  /// @returns `*this`.
  nlmsg_t& operator=(nlmsg_t&&) noexcept;

  /// @brief Free the underlying resource.
  ~nlmsg_t();

  /// @brief Return the underlying pointer.
  /// @returns the underlying managed pointer.
  [[nodiscard]] struct nl_msg* get_pointer() const noexcept;

// nl api wrappers ------------------------------------------------------------/

  /// @brief Put attribute into the message.
  /// @throw `std::runtime_error` When `nla_put_*()` call fail.
  void put_attr(nlattr_t);
  
  /// @brief Put more than one `nlattr_t` using a fold expression.
  /// @tparam Ts A `nlattr_t` parameter pack.
  /// @param[in] attr One or more nl attributes.
  template <is_nlattr_t... Ts> 
    void put_attr(Ts... attr);
  
  /// @brief Put a flag inside a netlink message.
  /// @throw `std::runtime_error` when `nla_put_flag()` call fail.
  void put_flag(nl80211_attrs);
  
  /// @brief Put more tha one `nl80211_attrs` flags using a fold expression.
  /// @tparam Ts A `nl80211_attrs` parameter pack.
  /// @param[in] flag One or more nl80211 flags.
  template <typename... Ts> requires (std::same_as<Ts,nl80211_attrs> && ...)
    void put_flag(Ts... flag);
  
  /// @brief Add a Generic Netlink header to message.
  /// @param[in] family Netlink family.
  /// @param[in] cmd Netlink command.
  /// @param[in] flags Flags to append.
  /// @throws `std::runtime_error` when `genlmsg_put()` call fail.
  void put_genl(int family, nl80211_commands cmd, int flags = 0);
  
  /// @brief Returns the actual netlink message header pointer.
  /// @returns The message header pointer.
  /// @details
  /// This is used to add a `nlmsg_flag` in this way: 
  /// \code msg.nlmsg_hdr()->nlmsg_flags |= NLM_F_DUMP; \endcode
  struct nlmsghdr* nlmsg_hdr() noexcept;

private:
  /// @brief Custom swap helper. Prevents recursive call of `std::swap()`.
  friend void swap(nlmsg_t& lhs, nlmsg_t& rhs) noexcept {
    std::swap(lhs.msgPtr_, rhs.msgPtr_);
  }
};


template <is_nlattr_t... Ts>
void nlmsg_t::put_attr(Ts... attrs)
{
  (..., this->put_attr(attrs));
}


template <typename... Ts> requires (std::same_as<Ts,nl80211_attrs> && ...)
void nlmsg_t::put_flag(Ts... flag)
{
  (..., this->put_flag(flag));
}


};  // end namespace nlpp


#endif // NLMSGT_HPP
