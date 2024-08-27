#ifndef GAME_SETUP_HPP
#define GAME_SETUP_HPP

#include <cstdint>


/// Used to show who owns the memory
template<typename T>
using Owner = T;

/// Used to show that pointer can't be nullptr
template<typename T>
using NotNull = T;

#define GAME_IS_LIKELY(condition) __builtin_expects(!!(condition), 1)
#define GAME_IS_UNLIKELY(condition) __builtin_expects(!!(condition), 0)

#endif // GAME_SETUP_HPP