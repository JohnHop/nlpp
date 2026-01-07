#if !defined(NLSOCKET_H)
#define NLSOCKET_H


/** 
 * @file nlsocket_t.hpp
 * Contains the `nlsocket_t` class definition.
 */

#include "nlpp.hpp"
#include "nlcb_t.hpp"
#include "nlmsg_t.hpp"

#include <netlink/socket.h>

#include <utility>


namespace nlpp {


/**
 * @brief Simple C++ wrapper around a `struct nl_sock` with RAII. 
 */
class nlsocket_t
{
  struct nl_sock* socketPtr_{}; // Underlying pointer
  bool connected_{};  // Connection status
  nlcb_t callback_;   // Callback to invoke after received a response

public:
  /// @brief Default ctor. Allocate a nl socket.
  /// @throws `std::system_error` when not enough memory available.
  nlsocket_t();

  /// @brief Quickly construct a socket and connect to a netlink protocol.
  /// @param[in] protocol Netlink protocol to connect.
  nlsocket_t(netlink_protocol_e protocol);

  /// @brief Move constructor.
  nlsocket_t(nlsocket_t&&) noexcept;

  /// @brief Move assignment operator.
  /// @returns `*this`.
  nlsocket_t& operator=(nlsocket_t&&) noexcept;

  /// @brief Close the connection (if needed) and free the resource.
  ~nlsocket_t();

  /// @brief Returns the underlying pointer.
  /// @returns The underlying managed pointer.
  [[nodiscard]] struct nl_sock* get_pointer() const noexcept;

  /// @brief Returns the connection status.
  /// @returns Tue if the socket is connected.
  [[nodiscard]] bool connected() const noexcept { return this->connected_; }


// libnl API -------------------------------------------------------------------

  /// @brief Connect to a netlink kernel subsystem using the socket.
  /// @param[in] protocol Netlink protocol to use.
  /// @throws `std::runtime_error` When connection fails.
  void connect(netlink_protocol_e protocol);

  /// @brief Close connection.
  void close();

  /// @brief Set callback.
  void set_cb(nlcb_t);

  /// @brief Finalize and transmit a Netlink message.
  /// @param[in] msg Netlink message to send.
  /// @throws `std::runtime_error` When `nl_send_auto()` fails.
  void send_auto(nlmsg_t const& msg);

  /// @brief Receive a set of messages.
  /// @param[in] cb Set of callbacks to control the behaviour.
  /// @throws `std::runtime_error` When `nl_recvmsgs()` fails.
  void recvmsgs(nlcb_t& cb);

private:
  /// @brief Custom swap helper. Prevents recursive call of `std::swap()`.
  friend void swap(nlsocket_t& lhs, nlsocket_t& rhs) noexcept
  {
    std::swap(lhs.socketPtr_, rhs.socketPtr_);
  }

// Netlink callbacks ----------------------------------------------------------/

  /// @brief Default error handler.
  static int error_handler(sockaddr_nl*, nlmsgerr*, void*) noexcept;

  /// @brief Default finish handler.
  static int finish_handler(nl_msg*, void*) noexcept;

  /// @brief default ack handler.
  static int ack_handler(nl_msg*, void*) noexcept;
};


};  // end `nlpp` namespace


#endif // NLSOCKET_H
