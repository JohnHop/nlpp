#include "nlcb_t.hpp"


#include <netlink/errno.h>

#include <system_error>


using namespace nlpp;


nlcb_t::nlcb_t(enum nl_cb_kind kind)
: cbPtr_{nl_cb_alloc(kind)}
{
  if(!cbPtr_) {
    throw std::system_error{ENOMEM, std::system_category(), 
      "failed to allocate netlink callback"};
  }
}


nlcb_t::nlcb_t(nlcb_t&& other) noexcept
{
  this->cbPtr_ = std::exchange(other.cbPtr_, nullptr);
}


nlcb_t& nlcb_t::operator=(nlcb_t&& rhs) noexcept
{
  nlcb_t move{std::move(rhs)};
  swap(*this, move);

  return *this;
}


/// @note It is safe to call `nl_cb_put(nullptr)`.
nlcb_t::~nlcb_t()
{
  nl_cb_put(this->cbPtr_);
}


struct nl_cb* nlcb_t::get_pointer() const noexcept 
{ 
  return this->cbPtr_; 
}


void nlcb_t::set(enum nl_cb_type type, enum nl_cb_kind kind, nl_recvmsg_msg_cb_t fun, void* arg)
{
  auto const result = nl_cb_set(this->cbPtr_, type, kind, fun, arg);
  if(result) {
    throw std::runtime_error{nl_geterror(result)};
  }
}


void nlcb_t::err(enum nl_cb_kind kind, nl_recvmsg_err_cb_t fun, void* arg)
{
  auto const result = nl_cb_err(this->cbPtr_, kind, fun, arg);
  if(result) {
    throw std::runtime_error{nl_geterror(result)};
  }
}