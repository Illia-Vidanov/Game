#include "Window.hpp"

#include <SDL2/SDL.h>

#include "Setup.hpp"
#include "Game.hpp"
#include "Core/Renderer.hpp"


namespace game
{
Window::Window(Game &game) noexcept : game_(game)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  width_ = display_mode.w / kStartupResDivFactor;
  height_ = display_mode.h / kStartupResDivFactor;

  sdl_window_ = SDL_CreateWindow(title_.c_str(),
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 width_,
                                 height_,
                                 SDL_WINDOW_RESIZABLE | game_.GetRenderer().GetSDLWindowFlags());
  
  assert(("Couldn't create sdl window", sdl_window_ != nullptr));
}

void Window::Exit() noexcept
{
  SDL_DestroyWindow(sdl_window_);
  SDL_Quit();
}

void Window::SetResolution(const Eigen::Vector2i &resolution) noexcept
{
  width_ = resolution(0);
  height_ = resolution(1);

  assert(("Widht and height of a window must be positive intagers", width_ > 0 && height_ > 0));

  SDL_SetWindowSize(sdl_window_, width_, height_);
}

void Window::SetTitle(const std::string &title) noexcept
{  
  title_ = title;

  SDL_SetWindowTitle(sdl_window_, title_.c_str());
}
}