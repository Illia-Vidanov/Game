// Entry point
#include "Core/Game.hpp"

int main(int argc, const char **argv)
{
  game::Game game(argc, argv);
  game.Run();

  return 0;
}