#if !defined (NLATTRT_HPP)
#define NLATTRT_HPP


/**
 * @file nlattr_t.hpp
 * Contains the `nlattr_t` struct definition.
 */


#include <concepts>
#include <cstdint>
#include <string>
#include <variant>

#include <linux/nl80211.h>


namespace nlpp {


/// @brief Variant for all Netlink attribute types for a message.
using nlattr_val_t 
  = std::variant<uint8_t,uint16_t,uint32_t,uint64_t,std::string>;


/// @brief Netlink Attribute struct definition.
struct nlattr_t
{
  nl80211_attrs name; ///< Attribute name 
  nlattr_val_t value; ///< Attribute value
};


/// @brief Concept for `nlattr_t` equality.
template <typename Type>
concept is_nlattr_t = std::same_as<Type,nlattr_t>;


};  // end namespace nlpp


#endif  // NLATTRT_HPP
