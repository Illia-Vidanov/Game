#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "Setup.hpp"

#include "Utils/FlagParser.hpp"
#include "Core/Renderer.hpp"
#include "Core/Window.hpp"
#include "Core/EventHandler.hpp"


class SDL_Window;

int main(int argc, char **argv);

namespace game
{
/// Main class of a game
class Game
{
  friend int ::main(int argc, char **argv);
public:
  [[nodiscard]] constexpr inline auto GetFlags() noexcept -> Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetFlags() const noexcept -> const Flags& { return flags_; }
  [[nodiscard]] constexpr inline auto GetWindow() noexcept -> Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetWindow() const noexcept -> const Window& { return window_; }
  [[nodiscard]] constexpr inline auto GetRenderer() noexcept -> Renderer& { return renderer_; }
  [[nodiscard]] constexpr inline auto GetRenderer() const noexcept -> const Renderer& { return renderer_; }

private:
  /// Initializes game
  Game(const int argc, const char * const *argv) noexcept;

  /// Main game loop
  void Run() noexcept;
  /// Call back of SDL_QuitEvent
  void QuitEvent();
  /// Close application
  void Exit() noexcept;

  bool running_ = false;
  Flags flags_;
  EventHandler events_;
  EventCleaner event_cleaner_;
  // In this case I use reference just because it's handy and the class surely shouldn't be moved or copied
  Renderer &renderer_;
  Window window_;
};
} // game

#endif // GAME_GAME_HPP