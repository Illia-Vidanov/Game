#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"
#include "Utils/FlagParser.hpp"


namespace game
{
/// @brief Main class of a game
class Game
{
public:
  /// @brief Initializes game
  ///
  /// Initializes: FlagParser
  /// @param argc from main
  /// @param argv from main
  Game(const int argc, const char **argv);

  /// @brief Main game loop
  void Run();

private:
  Flags flags_;
  bool running_ = false;
};
} // game

#endif // GAME_GAME_HPP