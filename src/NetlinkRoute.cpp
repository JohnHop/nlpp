#include "NetlinkRoute.hpp"


#include <stdexcept>
#include <string_view>

#include <netlink/route/link.h>

#include "nlpp.hpp"
#include "rtnl_link_t.hpp"


using namespace nlpp;


NetlinkRoute::NetlinkRoute()
{
  socket_.connect(netlink_protocol_e::route);
}


rtnl_link_t NetlinkRoute::get_link(if_identifier_t if_identifier)
{
  rtnl_link_t result;
  int err{};

  std::visit([socketPtr=socket_.get_pointer(), &result, &err](auto if_id)
  {
    struct rtnl_link* linkPtr;

    if constexpr(std::is_same_v<std::string_view,decltype(if_id)>) {
      err = rtnl_link_get_kernel(socketPtr, {}, if_id.data(), &linkPtr);
    }
    else if constexpr(std::is_same_v<if_index_t,decltype(if_id)>) {
      err = rtnl_link_get_kernel(socketPtr, if_id.get(), {}, &linkPtr);
    }

    result = rtnl_link_t{linkPtr};
  }, if_identifier);

  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }

  return result;
}


void NetlinkRoute::link_change(rtnl_link_t& link, rtnl_link_t& change, int flags)
{
  auto err = rtnl_link_change(socket_.get_pointer(), link.get_pointer(), change.get_pointer(), flags);
  if(err < 0) {
    throw std::runtime_error{nl_geterror(err)};
  }
}