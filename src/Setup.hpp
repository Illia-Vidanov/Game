#ifndef GAME_SETUP_HPP
#define GAME_SETUP_HPP

// Used to show who owns the memory
template<typename T>
using Owner = T;

#if !defined(NDEBUG) && defined(__GNUC__)
  #define GAME_IS_LIKELY(condition) __builtin_expects(!!(condition), 1)
  #define GAME_IS_UNLIKELY(condition) __builtin_expects(!!(condition), 0)

  #define GAME_EXPECTS(condition) (GAME_IS_LIKELY(condition) ? static_cast<void>(0) : __builtin_unreachable())
  #define GAME_ENSURES(condition) (GAME_IS_LIKELY(condition) ? static_cast<void>(0) : __builtin_unreachable())
#else
  #define GAME_IS_LIKELY(condition) (condition)
  #define GAME_IS_UNLIKELY(condition) (condition)

  #define GAME_EXPECTS(condition)
  #define GAME_ENSURES(condition)
#endif // NDEBUG

#if __cplusplus >= 202002L
  #define GAME_CONSTEXPR_CXX20 constexpr
#else
  #define GAME_CONSTEXPR_CXX20
#endif // c++ >= 20


#endif // GAME_SETUP_HPP