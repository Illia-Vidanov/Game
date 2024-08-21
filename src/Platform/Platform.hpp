#ifndef GAME_PLATFORM_HPP
#define GAME_PLATFORM_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
  #define GAME_OS_WIN
#endif // Windows macro check

#ifdef GAME_OS_WIN
  #include "Platform/Windows.hpp"
#endif // GAME_OS_WIN

#endif // GAME_PLATFORM_HPP