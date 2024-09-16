#ifndef GAME_SETUP_HPP
#define GAME_SETUP_HPP

#include <cstdint>

#include <tracy/Tracy.hpp>


/// Used to show who owns the memory
template<typename T>
using Owner = T;

/// Used to show that pointer can't be nullptr
template<typename T>
using NotNull = T;

#define GAME_IS_LIKELY(condition) __builtin_expect(!!(condition), 1)
#define GAME_IS_UNLIKELY(condition) __builtin_expect(!!(condition), 0)

#define GAME_WIN_OS defined(_WIN32) || defined(__CYGWIN__)
#define GAME_MAC_OS defined(__APPLE__)
#define GAME_LINUX_OS defined(__linux__)
#if !defined(GAME_WIN_OS) && !defined(GAME_MAC_OS) && !defined(GAME_OS_LINUX)
#error Unsupported platform
#endif

#endif // GAME_SETUP_HPP