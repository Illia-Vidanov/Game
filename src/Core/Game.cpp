#include "Game.hpp"

#include "SDL2/SDL.h"

#include "Setup.hpp"
#include "Utils/FlagParser.hpp"


namespace game
{
Game::Game(const int argc, const char **argv)
: flags_(argc, argv)
{
  //SDL_Init(SDL_INIT_EVERYTHING);
}

void Game::Run()
{
  running_ = true;
  while(running_)
  {
    running_ = false;
  }
}
} // game