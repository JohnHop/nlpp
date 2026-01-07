#if !defined(NLCBT_HPP)
#define NLCBT_HPP


/** 
 * @file nlcb_t.hpp
 * Contains the `nlcb_t` class definition.
 */


#include <netlink/handlers.h>

#include <utility>


namespace nlpp
{


/**
 * @brief Simple C++ wrapper around a `struct nlcb_t` with RAII.
 *
 * For documentation:
 * @see https://www.infradead.org/~tgr/libnl/doc/core.html#core_sk_cb
 * @see https://www.infradead.org/~tgr/libnl/doc/core.html#core_cb
 */
class nlcb_t
{
  struct nl_cb* cbPtr_{}; // Underlying pointer

public:
  /// @brief Default ctor. 
  nlcb_t() = default;

  /// @brief Alloc a nl callback.
  /// @param[in] kind Callback kind.
  /// @throws `std::system_error` with code ENOMEM.
  nlcb_t(enum nl_cb_kind kind);


  /// @brief Move ctor.
  nlcb_t(nlcb_t&&) noexcept;

  /// @brief Move assignment operator.
  /// @returns `*this`.
  nlcb_t& operator=(nlcb_t&&) noexcept;

  /// @brief Release the underlying resource.
  ~nlcb_t();

  /// @brief Returns the underlying pointer.
  /// @returns The underlying managed pointer.
  [[nodiscard]] struct nl_cb* get_pointer() const noexcept;

// nl api wrappers ------------------------------------------------------------/

  /// @brief Set up a callback for this set.
  /// @param[in] t Callback type.
  /// @param[in] k Callback kind.
  /// @throws `std::runtime_error` containing the description of the nl error code.
  void set(enum nl_cb_type t, enum nl_cb_kind k, nl_recvmsg_msg_cb_t, void*);
  
  /// @brief Set up the error callback for this set.
  /// @param[in] k Callback kind.
  /// @throws `std::runtime_error` containing the description of the nl error code.
  void err(enum nl_cb_kind k, nl_recvmsg_err_cb_t, void*);

private:
  /// @brief Custom swap helper. Prevents recursive call of `std::swap()`.
  friend void swap(nlcb_t& lhs, nlcb_t& rhs) noexcept
  {
    std::swap(lhs.cbPtr_, rhs.cbPtr_);
  }
};


};  // end namespace nlpp


#endif // NLCBT_HPP
