#include "Game.hpp"

#include "Setup.hpp"

#include <iostream>

#include <SDL2/SDL_keycode.h>

#include "Utils/FlagParser.hpp"
#include "Core/Renderer.hpp"
#include "Core/Window.hpp"
#include "Core/EventHandler.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
: flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
, event_cleaner_(events_)
, renderer_(Renderer::CreateRenderer())
, window_(*this)
{
  renderer_.Init(*this);

  events_.AddListener(event_cleaner_, kQuitEvent, this,
    [](__attribute__((unused)) const Event &event, void *data) -> bool
    {
      Game *game = reinterpret_cast<Game*>(data);
      game->QuitEvent();
      return true;
    });
  events_.AddListener(event_cleaner_, kKeyDownEvent, this,
    [](const Event &event, void *data) -> bool
    {
      if(event.GetKeycode() != SDLK_ESCAPE)
        return false;
      Game *game = reinterpret_cast<Game*>(data);
      game->QuitEvent();
      return true;
    });
}

void Game::Run() noexcept
{
  running_ = true;
  while(running_)
  {
    events_.DispatchSDLEvents();
    events_.DispatchEnquedEvents();
    renderer_.Render();
  }

  Exit();
}

void Game::QuitEvent()
{
  running_ = false;
}

void Game::Exit() noexcept
{
  renderer_.Exit();
  window_.Exit();
}
} // game