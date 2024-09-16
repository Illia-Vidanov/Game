#include "Renderer.hpp"

#include "Setup.hpp"

#include "Platform/OpenGL/OpenGLRenderer.hpp"


namespace game
{
Renderer &Renderer::CreateRenderer() noexcept
{
  ZoneScopedC(0x07dbd4);

  static OpenGLRenderer opengl_renderer;
  return opengl_renderer;
}
} // game