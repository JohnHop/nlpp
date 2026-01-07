#include "nlsocket_t.hpp"


#include <system_error>

#include <netlink/netlink.h>


using namespace nlpp;


nlsocket_t::nlsocket_t()
: socketPtr_{nl_socket_alloc()}
{
  if(!socketPtr_) {
    throw std::system_error{ENOMEM, std::system_category(),  
      "failed to allocate netlink socket."};
  }
}


nlsocket_t::nlsocket_t(netlink_protocol_e protocol)
: nlsocket_t{}
{
  this->connect(protocol);
}


nlsocket_t::nlsocket_t(nlsocket_t&& other) noexcept
{
  socketPtr_ = std::exchange(other.socketPtr_, nullptr);
  connected_ = std::exchange(other.connected_, {});
  callback_ = std::exchange(other.callback_, {});
}


nlsocket_t& nlsocket_t::operator=(nlsocket_t&& other) noexcept
{
  nlsocket_t move{std::move(other)};  // copy-and-swap idiom
  swap(*this, move);
  
  return *this;
}


nlsocket_t::~nlsocket_t()
{
  nl_socket_free(socketPtr_);
}


struct nl_sock* nlsocket_t::get_pointer() const noexcept 
{ 
  return socketPtr_;
}


void nlsocket_t::connect(netlink_protocol_e protocol)
{
  int const connectResult 
    = nl_connect(socketPtr_, std::to_underlying(protocol));
    
  if(connectResult) {
    throw std::system_error{connectResult, std::system_category()};
  }

  this->connected_ = true;
}


void nlsocket_t::close()
{
  nl_close(socketPtr_);
  this->connected_ = false;
}


void nlsocket_t::set_cb(nlcb_t cb)
{
  this->callback_ = std::move(cb);
  nl_socket_set_cb(socketPtr_, this->callback_.get_pointer());
}


void nlsocket_t::send_auto(nlmsg_t& msg)
{
  int err = nl_send_auto(socketPtr_, msg.get_pointer());
  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }
}


// TODO: valuta se mettere questa roba nel costruttore di `nlsocket_t` invece di
// ripeterla ogni volta.

void nlsocket_t::recvmsgs(nlcb_t& cb)
{
  int err = 1;

  cb.err(NL_CB_CUSTOM, nlsocket_t::error_handler, &err);
  cb.set(NL_CB_FINISH, NL_CB_CUSTOM, nlsocket_t::finish_handler, &err);
  cb.set(NL_CB_ACK, NL_CB_CUSTOM, nlsocket_t::ack_handler, &err);

  while(err > 0) {
    nl_recvmsgs(socketPtr_, cb.get_pointer());
  }

  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }
}


int nlsocket_t::error_handler(sockaddr_nl*, nlmsgerr* err, void* arg) noexcept
{
	int* ret = reinterpret_cast<int*>(arg);
	*ret = err->error;

	return NL_STOP;
}


int nlsocket_t::finish_handler(nl_msg*, void* arg) noexcept
{
	int* ret = reinterpret_cast<int*>(arg);
	*ret = 0;

	return NL_SKIP;
}


int nlsocket_t::ack_handler(nl_msg*, void* arg) noexcept
{
	int* ret = reinterpret_cast<int*>(arg);
	*ret = 0;

	return NL_STOP;
}