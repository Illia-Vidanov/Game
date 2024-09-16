#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include "Setup.hpp"


namespace game
{
class Game;

class Renderer
{
public:
  /// Function factory to create only one renderer
  /// return Renderer according to current best fit
  static Renderer &CreateRenderer() noexcept;
  /// Initialize renderer with window
  virtual void Init(Game &game) noexcept = 0;
  /// Add mesh to a rendering queue
  /// First added meshes are rendered first
  virtual void AddToQueue() noexcept = 0;
  /// Render meshes from queue
  virtual void Render() noexcept = 0;
  /// Deinit renderer and clear memory
  virtual void Exit() noexcept = 0;

  /// Used to get SDL_WINDOW_OPENGL or SDL_WINDOW_VUKAN or ...
  virtual auto GetSDLWindowFlags() const noexcept -> int = 0;
};
} // game

#endif // GAME_RENDERER_HPP