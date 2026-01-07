#include "nlmsg_t.hpp"

#include <netlink/attr.h>
#include <netlink/errno.h>
#include <netlink/genl/genl.h>

#include <system_error>


using namespace nlpp;


nlmsg_t::nlmsg_t()
: msgPtr_{nlmsg_alloc()}
{
  if(!msgPtr_) {
    throw std::system_error{ENOMEM, std::system_category(), 
      "unable to allocate netlink message"};
  }
}


nlmsg_t::nlmsg_t(int family, nl80211_commands cmd, int flags)
: nlmsg_t() // delegate ctor, allocate a `nlmsg_t` first
{
  this->put_genl(family, cmd, flags);
}


nlmsg_t::nlmsg_t(nlmsg_t&& other) noexcept
{
  this->msgPtr_ = std::exchange(other.msgPtr_, nullptr);
}


nlmsg_t& nlmsg_t::operator=(nlmsg_t&& rhs) noexcept
{
  nlmsg_t moved{std::move(rhs)};
  swap(*this, moved);

  return *this;
}


nlmsg_t::~nlmsg_t()
{
  nlmsg_free(this->msgPtr_);
}


struct nl_msg* nlmsg_t::get_pointer() const noexcept 
{ 
  return this->msgPtr_; 
}


void nlmsg_t::put_attr(nlattr_t attr)
{
  int err 
    = std::visit([msgPtr=this->msgPtr_, name=attr.name](auto value) -> int
    {
      if constexpr(std::is_same_v<decltype(value), uint8_t>) {
        return nla_put_u8(msgPtr, name, value);
      }
      else if constexpr(std::is_same_v<decltype(value), uint16_t>) {
        return nla_put_u16(msgPtr, name, value);
      }
      else if constexpr(std::is_same_v<decltype(value), uint32_t>) {
        return nla_put_u32(msgPtr, name, value);
      }
      else if constexpr(std::is_same_v<decltype(value), uint64_t>) {
        return nla_put_u64(msgPtr, name, value);
      }
      else if constexpr(std::is_same_v<decltype(value), std::string>) {
        return nla_put_string(msgPtr, name, value.data());
      }
    }, attr.value);

  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }
}


void nlmsg_t::put_flag(nl80211_attrs flag)
{
  int err = nla_put_flag(msgPtr_, NL80211_ATTR_SPLIT_WIPHY_DUMP);

  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }
}


// TODO: missing remaining parameters.
void nlmsg_t::put_genl(int family, nl80211_commands cmd, int flags)
{
  void* errPtr = genlmsg_put(msgPtr_, 0, 0, family, 0, flags, cmd, 0);
  if(!errPtr) {
    throw std::runtime_error{"genlmsg_put() error"};
  }
}


struct nlmsghdr* nlmsg_t::nlmsg_hdr() noexcept
{ 
  return ::nlmsg_hdr(msgPtr_); 
}