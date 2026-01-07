#include "nlcache_t.hpp"


#include <array>
#include <cstring>
#include <stdexcept>
#include <string>

#include <netlink/route/link.h>


using namespace nlpp;


nlcache_t::nlcache_t(nlsocket_t& nlsocket, int family)
{
  //? Verifica connessione?

  int const error 
    = rtnl_link_alloc_cache(nlsocket.get_pointer(), family, &cachePtr_);
  if(error) {
    throw std::runtime_error{nl_geterror(error)};
  }
}


nlcache_t::nlcache_t(nlcache_t&& other) noexcept
{
  cachePtr_ = std::exchange(other.cachePtr_, nullptr);
}


nlcache_t& nlcache_t::operator=(nlcache_t&& rhs) noexcept
{
  nlcache_t move{std::move(rhs)};
  swap(*this, move);

  return *this;
}


nlcache_t::~nlcache_t()
{
  nl_cache_free(cachePtr_);
}


struct nl_cache* nlcache_t::get_pointer() const noexcept
{ 
  return this->cachePtr_;
}


std::optional<rtnl_link_t> nlcache_t::get_link(if_index_t ifindex)
{
  auto* resultPtr = rtnl_link_get(cachePtr_, ifindex.get());

  return resultPtr ? std::make_optional(rtnl_link_t{resultPtr}) : std::nullopt;
}


std::optional<rtnl_link_t> nlcache_t::get_link(std::string_view ifname)
{
  auto* resultPtr = rtnl_link_get_by_name(cachePtr_, ifname.data());

  return resultPtr ? std::make_optional(rtnl_link_t{resultPtr}) : std::nullopt;
}


std::string nlcache_t::i2name(if_index_t ifindex)
{
  std::array<char,100> dst; //? max array size?

  auto resultPtr = rtnl_link_i2name(cachePtr_, ifindex.get(), dst.data(), dst.size());

  return resultPtr ? std::string(dst.data()) : std::string{};
}


std::optional<if_index_t> nlcache_t::name2i(std::string_view ifname)
{
  auto result = rtnl_link_name2i(cachePtr_, ifname.data());

  return result 
    ? std::make_optional(if_index_t{static_cast<unsigned int>(result)}) 
      : std::nullopt;
}