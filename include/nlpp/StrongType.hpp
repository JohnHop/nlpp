#if !defined(STRONGTYPE_HPP)
#define STRONGTYPE_HPP

/**
 * @file StrongType.hpp
 * Definition for `StrongType` class and his skills.
 */

namespace nlpp {

/**
 * @brief Strong type template class.
 */
template <typename Type, typename Tag, template <typename> class... Skills>
class StrongType : private Skills<StrongType<Type, Tag, Skills...>>...
{
  Type value_;

public:
  using value_type      = Type;               ///< Type
  using reference       = value_type &;       ///< Type&
  using const_reference = value_type const &; ///< Type const&

  /// @brief Constructs a StrongType from his underlying value.
  /// @param[in] v The value to wrap.
  constexpr explicit StrongType(Type const &v) : value_{v} {}

  /// @brief Get the underlying value.
  /// @returns The reference to underlying value.
  constexpr reference get() { return value_; }

  /// @brief Get the underlying value.
  /// @returns The const reference to underlying value.
  constexpr const_reference get() const { return value_; }

private:
  /// @brief Enable equally comparision by default.
  [[nodiscard]] friend bool operator==(StrongType const &lhs,
                                       StrongType const &rhs) noexcept {
    return lhs.value_ == rhs.value_;
  }
};


/// @brief Enable `operator<` for `StrongType`.
template <typename Derived> struct LessComparable 
{
  /// @brief Execute comparision with operator < for a `StrongType`.
  /// @param[in] lhs Left operand.
  /// @param[in] rhs Right operand.
  /// @returns True if left inner value is less then rhs inner value.
  [[nodiscard]] friend bool operator<(Derived const &lhs,
                                      Derived const &rhs) noexcept {
    return lhs.value_ < rhs.value;
  }
};


} // namespace nlpp


#endif // STRONGTYPE_HPP
