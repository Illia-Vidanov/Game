#ifndef GAME_MATH_HPP
#define GAME_MATH_HPP

#include "Setup.hpp"

#include <type_traits>
#include <cmath>

#include <gcem/gcem.hpp>

#include "Math/Constants.hpp"

namespace game
{
// If type is custom it's better to leave it as is
template<typename T>
using ReturnFloatType = std::conditional_t<std::is_integral_v<T>, DefFloatType, T>;


template<typename T>
constexpr inline ReturnFloatType<T> DegreesToRadians(T degrees)
{ return degrees * (kPI / 180); }

template<typename T>
constexpr inline ReturnFloatType<T> RadiansToDegrees(T radians)
{ return radians * (180 / kPI); }


template<typename T, typename U>
constexpr inline bool AreSame(T t, U u)
{ return gcem::abs(t - u) < kElipson; }

// Shorthand of writing (std::numeric_limits<T>::digits10 + 1)
template<typename T>
constexpr inline static int kMaxDigits = std::numeric_limits<T>::digits10 + 1;


namespace detail
{
template<typename T>
constexpr inline T kFastPower10PositiveLookup[20] =
{
    1, 10, 100, kMaxDigits<T> > 3 ? 1000 : 0,
    kMaxDigits<T> > 4 ?  gcem::pow<T, T>(10, 4) : 0,  kMaxDigits<T> > 5 ?  gcem::pow<T, T>(10, 5) : 0,  kMaxDigits<T> > 6 ?  gcem::pow<T, T>(10, 6) : 0,  kMaxDigits<T> > 7 ?  gcem::pow<T, T>(10, 7) : 0,
    kMaxDigits<T> > 8 ?  gcem::pow<T, T>(10, 8) : 0,  kMaxDigits<T> > 9 ?  gcem::pow<T, T>(10, 9) : 0,  kMaxDigits<T> > 10 ? gcem::pow<T, T>(10, 10) : 0, kMaxDigits<T> > 11 ? gcem::pow<T, T>(10, 11) : 0, 
    kMaxDigits<T> > 12 ? gcem::pow<T, T>(10, 12) : 0, kMaxDigits<T> > 13 ? gcem::pow<T, T>(10, 13) : 0, kMaxDigits<T> > 14 ? gcem::pow<T, T>(10, 14) : 0, kMaxDigits<T> > 15 ? gcem::pow<T, T>(10, 15) : 0,
    kMaxDigits<T> > 16 ? gcem::pow<T, T>(10, 16) : 0, kMaxDigits<T> > 17 ? gcem::pow<T, T>(10, 17) : 0, kMaxDigits<T> > 18 ? gcem::pow<T, T>(10, 18) : 0, kMaxDigits<T> > 19 ? gcem::pow<T, T>(10, 19) : 0
};

constexpr inline DefFloatType kFastPower10NegativeLookup[20] =
{
    static_cast<DefFloatType>(1), static_cast<DefFloatType>(1) / 10, static_cast<DefFloatType>(1) / 100, static_cast<DefFloatType>(1) / 1000,
    static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 4),  static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 5),  static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 6),  static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 7),
    static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 8),  static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 9),  static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 10), static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 11), 
    static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 12), static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 13), static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 14), static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 15),
    static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 16), static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 17), static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 18), static_cast<DefFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 19)
};
} // detail

// Function to get power of 10 with lookup
// If exponent might be floatin-point specialize return value
// Example: FastPower10<float>(2.3);
// If exp > 19 or exp < -19, 1 is returned
template<typename T = DefUIntType, typename U>
constexpr inline T FastPower10(const U exp)
{
    if constexpr(!std::is_integral_v<U>)
        return gcem::pow(T(10), exp);
    else if(std::is_signed_v<U>)
    {
        if(exp < 0)
        {
            if(exp < -19)
                return 1;

            return static_cast<T>(detail::kFastPower10NegativeLookup[-exp]);
        }
    }

    if(exp > 19)
        return 1;

    return detail::kFastPower10PositiveLookup<T>[exp];
}


// Gets digit at index from number
// If index is invalid 0 returned
// If value is floating point, fraction part is ignored, by casting value to long long
// Indexing starts at 0 and equals the same as (x / 10^index) % 10
template<typename T = uint8_t, typename U, typename V, std::enable_if_t<!std::is_floating_point_v<U>, bool> = true>
constexpr inline T GetDigit(const U u, const V index)
{
    // We might get division by 0 if we skip this line
    if(index < 0)
        return 0;
    return (gcem::abs(u) / FastPower10<uint64_t>(index)) % U(10);
}

template<typename T = uint8_t, typename U, typename V, std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
constexpr inline T GetDigit(const U u, const V index)
{
    return GetDigit(static_cast<long long>(u), index);
}


// Get substring from number
// Same as (number / 10^first) % 10^last
// first number inclusive, last exclusive
// Example: first = 1, last = 5, number = 6543210 => 4321;
//          first = 4, last = 8, number = -6543210 => 654;
//          first = -10, last = 5, number = -6543210 => 43210;
// If number is floating point, fraction part is ignored by casting value to long long
template<typename T, typename U, std::enable_if_t<!std::is_floating_point_v<T>, bool> = true>
constexpr inline T GetDigitSubstring(const T number, U first, const U last)
{
    // I am not sure about this line.
    // It might be better to throw an error, but it means we need to check for it anyway, so I would prefer doing it this way
    // If we'll ignore the fact of negative numbers we'll get arithmetic exception that is hard to debug
    if(first < 0)
        first = 0;
    if(last < 0)
        return 0;
    if(first >= last)
        return 0;

    return (number / IntegralPower(10, first)) % IntegralPower(10, last - first);
}

template<typename T, typename U, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr inline T GetDigitSubstring(const T number, const U first, const U last)
{
    return GetDigitSubstring(static_cast<long long>(number), first, last);
}


namespace detail
{
template <typename T>
constexpr inline DefUIntType DigitCountImpl(T t)
{
    if(t < T(0))
        return DigitCountImpl(-t);
    DefUIntType digits = 0;
    while(T(0) < t)
    {
        digits++;
        t = t / 10;
    }
    return digits;
}

template<>
inline DefUIntType DigitCountImpl<char>(char t)
{
    if(t < 0)
        return DigitCountImpl<char>(-t);
    
    static const char x[128] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
    return x[static_cast<unsigned char>(t)];
}
template<>
inline DefUIntType DigitCountImpl<unsigned char>(unsigned char t)
{
    static const unsigned char x[256] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
    return x[t];
}

template<>
constexpr inline DefUIntType DigitCountImpl<short>(short t)
{
    if(t < 0)
        return DigitCountImpl<short>(-t);
    
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t > 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    else
    {
        if(t >= 10)
            return 2;
        return 1;
    }
}
template<>
constexpr inline DefUIntType DigitCountImpl<unsigned short>(unsigned short t)
{
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t > 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    else
    {
        if(t >= 10)
            return 2;
        return 1;
    }
}

template<>
constexpr inline DefUIntType DigitCountImpl<int>(int t)
{
    if(t < 0) return DigitCountImpl<int>(-t);

    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t > 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if(t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}

template<>
constexpr inline DefUIntType DigitCountImpl<unsigned int>(unsigned int t)
{
    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t > 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if(t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}

template<>
constexpr inline DefUIntType DigitCountImpl<long long>(long long t)
{
    if(t < 0)
        return DigitCountImpl<long long>(-t);

    if(t >= 10000000000)
    {
        if(t >= 100000000000000)
        {
            if(t >= 10000000000000000)
            {
                if(t >= 100000000000000000)
                {
                    if(t >= 1000000000000000000)
                        return 19;
                    return 18;
                }
                return 17;
            }
            if(t >= 1000000000000000)
                return 16;
            return 15;
        } 
        if(t >= 1000000000000)
        {
            if(t >= 10000000000000)
                return 14;
            return 13;
        }
        if(t >= 100000000000)
            return 12;
        return 11;
    }
    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}
template<>
constexpr inline DefUIntType DigitCountImpl<unsigned long long>(unsigned long long t)
{
    if(t >= 10000000000)
    {
        if(t >= 1000000000000000)
        {
            if(t >= 100000000000000000)
            {
                if(t >= 1000000000000000000)
                {
                    if(t >= 10000000000000000000u)
                        return 20;
                    return 19;
                }
                return 18;
            }
            if(t >= 10000000000000000)
                return 17;
            return 16;
        } 
        if(t >= 1000000000000)
        {
            if(t >= 10000000000000)
            {
                if(t > 100000000000000)
                    return 15;
                return 14;
            }
            return 13;
        }
        if(t >= 100000000000)
            return 12;
        return 11;
    }
    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}
} // detail

// Get number of whole digits in any type
// 0 = 0 digits
template<typename T = DefUIntType, typename U>
constexpr inline T DigitCount(U number)
{
    return number == 0 ? 0 : static_cast<T>(detail::DigitCountImpl(number));
}


//template<typename T>
//constexpr inline bool IsMathematicallyIntegral(T t)
//{
//    DefFloatType result = gcem::cos(2 * t * GCEM_PI);
//    return result == 1;
//}
//
//
//template<typename T = DefUIntType, typename U>
//constexpr inline T DigitCountAfterPoint(U number)
//{
//    T result = T(0);
//    while(!IsMathematicallyIntegral(number))
//}

} // game


#endif // GAME_MATH_HPP