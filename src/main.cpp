// Entry point
#include "SDL2/SDL.h" // For main function
#include "Core/Game.hpp"

int main(int argc, char **argv)
{
  game::Game game(argc, argv);
  game.Run();

  return 0;
}