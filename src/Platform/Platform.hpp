#ifndef GAME_PLATFORM_HPP
#define GAME_PLATFORM_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
  #define GAME_OS_WIN
#endif

#ifdef GAME_OS_WIN
  #include "Platform/Windows/Windows.hpp"
#endif

#endif // GAME_PLATFORM_HPP