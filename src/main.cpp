/*
Todo:
1. Change event naming
6. Renderer version log
*/
#include "Setup.hpp"

#include "Core/Game.hpp"

int main(int argc, char **argv)
{
  ZoneScopedC(0xd10834);

  game::Game game(argc, argv);
  game.Run();

  return 0;
}