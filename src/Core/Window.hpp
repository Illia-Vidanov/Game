#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <string>

#include "Setup.hpp"

#include <Eigen/Dense>


class SDL_Window;

namespace game
{
class Game;

class Window
{
public:
  Window(Game &game) noexcept;

  void Exit() noexcept;

  [[nodiscard]] constexpr inline auto GetWidth() noexcept -> int { return width_; }
  [[nodiscard]] constexpr inline auto GetHeight() noexcept -> int { return height_; }
  [[nodiscard]] inline auto GetResolution() noexcept -> Eigen::Vector2i { return Eigen::Vector2i{width_, height_}; }
  void SetResolution(const Eigen::Vector2i &resolution) noexcept;
  
  [[nodiscard]] constexpr inline auto GetSDLWindow() noexcept -> SDL_Window* { return sdl_window_; }

  [[nodiscard]] inline auto GetTitle() noexcept -> const std::string & { return title_; }
  void SetTitle(const std::string &title) noexcept;

private:
  Game &game_;

  static constexpr inline int kStartupResDivFactor = 3;
  int width_;
  int height_;
  std::string title_ = "Game";

  SDL_Window *sdl_window_;
};
}

#endif