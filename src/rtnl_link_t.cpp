#include "rtnl_link_t.hpp"
#include "nlpp.hpp"


#include <optional>
#include <string>
#include <format>
#include <utility>


using namespace nlpp;


rtnl_link_t::rtnl_link_t()
: linkPtr_{rtnl_link_alloc()}
{
  if(!linkPtr_) {
    throw std::system_error{ENOMEM, std::system_category(), 
      "unable to allocate rtnl link"};
  }
}


rtnl_link_t::rtnl_link_t(struct rtnl_link* otherPtr)
{
  if(!otherPtr) {
    throw std::logic_error{"invalid struct rtnl_link pointer"};
  }

  linkPtr_ = otherPtr;
}


rtnl_link_t::rtnl_link_t(rtnl_link_t&& other) noexcept
{
  linkPtr_ = std::exchange(other.linkPtr_, nullptr);
}


rtnl_link_t& rtnl_link_t::operator=(rtnl_link_t&& rhs) noexcept
{
  rtnl_link_t move{std::move(rhs)};
  swap(*this, move);

  return *this;
}


rtnl_link_t::~rtnl_link_t()
{
  rtnl_link_put(linkPtr_);
}


struct rtnl_link* rtnl_link_t::get_pointer() const noexcept 
{ 
  return this->linkPtr_;
}


std::string rtnl_link_t::to_string()
{
  auto index = this->get_index();

  return std::format("name: {}, index: {}, flags: {}, state: {}",
    this->name(),
    index.has_value() ? std::to_string(index.value().get()) : "",
    "{" + ::to_string(this->flags()) + "}",
    ::to_string(this->operstate()));
}


std::string rtnl_link_t::name() noexcept
{
  return rtnl_link_get_name(linkPtr_);
}


std::optional<if_index_t> rtnl_link_t::get_index() noexcept
{
  std::optional<if_index_t> result;

  auto index = rtnl_link_get_ifindex(linkPtr_);
  if(index) {
    result = std::make_optional<if_index_t>(static_cast<uint32_t>(index));
  }

  return result;
}


if_operstate_e rtnl_link_t::operstate() noexcept
{
  return if_operstate_e{rtnl_link_get_operstate(linkPtr_)};
}


if_flags_t rtnl_link_t::flags() noexcept
{
  return if_flags_t{rtnl_link_get_flags(linkPtr_)};
}


void rtnl_link_t::set_flags(if_flags_t flags) noexcept
{
  rtnl_link_set_flags(linkPtr_, 
    static_cast<unsigned int>(flags.get().to_ulong()));
}


void rtnl_link_t::unset_flags(if_flags_t flags) noexcept
{
  rtnl_link_unset_flags(linkPtr_, 
    static_cast<unsigned int>(flags.get().to_ulong()));
}