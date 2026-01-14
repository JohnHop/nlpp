#include "NetlinkRoute.hpp"


#include <stdexcept>

#include <netlink/route/link.h>

#include "nlpp.hpp"
#include "rtnl_link_t.hpp"


using namespace nlpp;


NetlinkRoute::NetlinkRoute()
{
  socket_.connect(netlink_protocol_e::route);
}


rtnl_link_t NetlinkRoute::get_kernel(if_index_t ifindex)
{
  struct rtnl_link* linkPtr;  // out argument for `rtnl_link_get_kernel()`
    
  int err 
    = rtnl_link_get_kernel(socket_.get_pointer(), ifindex.get(), {}, &linkPtr);

  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }

  return rtnl_link_t{linkPtr};
}


rtnl_link_t NetlinkRoute::get_kernel(std::string const& ifname)
{
  struct rtnl_link* linkPtr;  // out argument for `rtnl_link_get_kernel()`
  
  int err 
    = rtnl_link_get_kernel(socket_.get_pointer(), {}, ifname.c_str(), &linkPtr);

  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }

  return rtnl_link_t{linkPtr};
}


void NetlinkRoute::link_change(rtnl_link_t& link, rtnl_link_t& change, int flags)
{
  auto err = rtnl_link_change(socket_.get_pointer(), link.get_pointer(), change.get_pointer(), flags);
  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }
}