#include "Game.hpp"

#include "Setup.hpp"

#include <SDL2/SDL_keycode.h>

#include "Utils/Logger.hpp"
#include "Utils/FlagParser.hpp"
#include "Core/Renderer.hpp"
#include "Core/Window.hpp"
#include "Core/EventHandler.hpp"
#include "Utils/Logger.hpp"


namespace game
{
Game::Game(const int argc, const char * const *argv) noexcept
: running_(([]{
    
  }(), false)) // pre initialization functions
, flags_(argc, argv) // args are UTF8 encoded because we use SDL2main
, event_cleaner_(events_)
, renderer_(Renderer::CreateRenderer())
, window_(*this)
{
  ZoneScopedC(0xb3041b);

  #if defined(GAME_WIN_OS)
    GAME_LOG(LogType::Info) << "OS: Windows";
  #elif defined(GAME_LINUX_OS)
    GAME_LOG(LogType::Info) << "OS: Linux";
  #elif defined(GAME_MAC_OS)
    GAME_LOG(LogType::Info) << "OS: Mac OS X";
  #endif

  renderer_.Init(*this);

  events_.AddListener(event_cleaner_, kQuitEvent, this,
    [](__attribute__((unused)) const Event &event, void *data) -> bool
    {
      reinterpret_cast<Game*>(data)->QuitEvent();
      return true;
    });
  events_.AddListener(event_cleaner_, kKeyDownEvent, this,
    [](const Event &event, void *data) -> bool
    {
      if(event.GetKeycode() != SDLK_ESCAPE)
        return false;
      reinterpret_cast<Game*>(data)->QuitEvent();
      return true;
    });
}

void Game::Run() noexcept
{
  {
  ZoneScopedC(0xb3041b);

  running_ = true;
  while(running_)
  {
    events_.DispatchSDLEvents();
    events_.DispatchEnquedEvents();

    renderer_.Render();

    FrameMark;
  }
  }

  Exit();
}

void Game::QuitEvent()
{
  ZoneScopedC(0xb3041b);

  running_ = false;
}

void Game::Exit() noexcept
{
  ZoneScopedC(0xb3041b);

  renderer_.Exit();
  window_.Exit();
}
} // game