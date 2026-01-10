#include "WifiDevice.hpp"


#include <format>


WifiDevice::WifiDevice(std::string_view if_name, nlpp::if_type_e if_type)
{ 
  ifindex_ = nlroute_.get_link(if_name).get_index().value();

  // optionally set the interface type
  if(if_type != nlpp::if_type_e::unspecified) {
    this->set_type(if_type);
  }
}


std::string WifiDevice::get_name()
{

  return nlroute_.get_link(ifindex_).name();
}


bool WifiDevice::is_up() noexcept
{
  return nlroute_.get_link(ifindex_).flags().get().to_ulong() 
    & std::to_underlying(nlpp::if_flag_e::up);
}


nlpp::if_type_e WifiDevice::get_type()
{
  // FIXME: always check optionals!
  return nlgeneric_.get_interface(nlroute_.get_link(ifindex_).get_index().value()).type;
}


std::optional<nlpp::frequency_t> WifiDevice::get_frequency()
{
  auto result = nlgeneric_.get_interface(
    nlroute_.get_link(ifindex_).get_index().value()).wiphy_freq;
  
  return result;
}


std::string WifiDevice::to_string()
{
  auto link = nlroute_.get_link(ifindex_);

  return std::format("{}, state: {}, flags: {}", 
    nlpp::to_string(nlgeneric_.get_interface(link.get_index().value())),
    nlpp::to_string(link.operstate()), 
    nlpp::to_string(link.flags()) );
}


void WifiDevice::put_up()
{
  auto current = nlroute_.get_link(ifindex_);
  nlpp::rtnl_link_t change;

  change.set_flags(nlpp::if_flags_t{std::to_underlying(nlpp::if_flag_e::up)});
  nlroute_.link_change(current, change);
}


void WifiDevice::put_down()
{
  auto current = nlroute_.get_link(ifindex_);
  nlpp::rtnl_link_t change;

  change.unset_flags(nlpp::if_flags_t{std::to_underlying(nlpp::if_flag_e::up)});
  nlroute_.link_change(current, change);
}


void WifiDevice::set_type(nlpp::if_type_e type)
{
  this->put_down();
  nlgeneric_.set_if_type(nlroute_.get_link(ifindex_).name(), type);
  this->put_up();
}


void WifiDevice::set_frequency(nlpp::frequency_t freq)
{
  this->nlgeneric_.set_if_frequency(nlroute_.get_link(ifindex_).name(), freq);
}