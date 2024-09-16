#ifndef GAME_OPEN_GL_RENDERER
#define GAME_OPEN_GL_RENDERER

#include "Setup.hpp"

#include <SDL2/SDL_video.h>

#include "Core/Renderer.hpp"


namespace game
{
class Window;
class Game;

class OpenGLRenderer : public Renderer
{
  friend Renderer;
public:
  void Init(Game &game) noexcept override;
  /// Add mesh to render queue
  /// First added meshes will be rendered first
  void AddToQueue() noexcept override {}
  void Render() noexcept override;
  void Exit() noexcept override;

  int GetSDLWindowFlags() const noexcept override { return SDL_WINDOW_OPENGL; }

private:
  OpenGLRenderer() noexcept;

  Game *game_ = nullptr;
  void *context_; // even in implementation SDL_GLContext is just typedef to void* 
};
} // game

#endif