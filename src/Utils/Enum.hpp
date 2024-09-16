#ifndef GAME_ENUM_HPP
#define GAME_ENUM_HPP

#include <type_traits>
#include <iostream>

#include "Setup.hpp"

namespace game
{
namespace detail
{
	template<typename T>
	constexpr bool kEnumOrArithmetic = std::is_enum_v<T> || std::is_arithmetic_v<T>;

	template<typename T, typename U>
	constexpr bool kCanDefineEnumOperator = !(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) && kEnumOrArithmetic<T> && kEnumOrArithmetic<U>;
}

template<typename T, std::enable_if_t<!detail::kEnumOrArithmetic<T>, bool> = true> inline constexpr T ToUnderlying(T value)
{ static_assert(false, "Can't cast to underlying type non enum"); }

template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
inline constexpr T ToUnderlying(T value)
{ return value; }

template<typename T>
inline constexpr std::underlying_type_t<T> ToUnderlying(T value)
{ return static_cast<std::underlying_type_t<T>>(value); }


namespace detail
{
template<typename T, bool = std::is_enum_v<T>>
struct UnderlyingTypeImpl
{ using type = T; };

template<typename T>
struct UnderlyingTypeImpl<T, true>
{ using type = std::underlying_type_t<T>; };
}

template<typename T>
struct UnderlyingType
{ using type = detail::UnderlyingTypeImpl<T>; };

template<typename T>
using underlying_type = typename UnderlyingType<T>::type;


// TODO: SFINAE if defenition of operator between T and U exists

// We need here std::enable_if_t in order to not overload existing operators
// Value can also be arithmetic if we want to apply it between value and enum

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr T operator|(T value1, U value2) 
{return static_cast<T>(ToUnderlying(value1) | ToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr T operator&(T value1, U value2) 
{ return static_cast<T>(ToUnderlying(value1) & ToUnderlying(value2)); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr T operator^(T value1, U value2) 
{ return static_cast<T>(ToUnderlying(value1) ^ ToUnderlying(value2)); }


template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr bool operator==(T value1, U value2) 
{ return ToUnderlying(value1) == ToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr bool operator!=(T value1, U value2) 
{ return ToUnderlying(value1) != ToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr bool operator<(T value1, U value2) 
{ return ToUnderlying(value1) < ToUnderlying(value2); }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr bool operator>(T value1, U value2) 
{ return ToUnderlying(value1) > ToUnderlying(value2); }


template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr T &operator|=(T &value1, U value2)
{ value1 = static_cast<T>(ToUnderlying(value1) | ToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr T &operator&=(T &value1, U value2)
{ value1 = static_cast<T>(ToUnderlying(value1) & ToUnderlying(value2)); return value1; }

template<typename T, typename U, std::enable_if_t<detail::kCanDefineEnumOperator<T, U>, bool> = true>
inline constexpr T &operator^=(T &value1, U value2)
{ value1 = static_cast<T>(ToUnderlying(value1) ^ ToUnderlying(value2)); return value1; }


template<typename T, typename std::enable_if_t<std::is_enum_v<T>, bool> = true> inline std::ostream& operator<<(std::ostream& os, T self)
{ return os << ToUnderlying(self); }
} // game

#endif // GAME_ENUM_HPP