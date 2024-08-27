#include "Renderer.hpp"

#include "Setup.hpp"
#include "Platform/OpenGL/OpenGLRenderer.hpp"


namespace game
{
Renderer &Renderer::CreateRenderer() noexcept
{
  static OpenGLRenderer opengl_renderer;
  return opengl_renderer;
}
} // game