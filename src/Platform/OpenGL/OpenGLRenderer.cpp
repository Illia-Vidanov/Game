#include "OpenGLRenderer.hpp"

#include "glad/glad.h"
#include "SDL2/SDL.h"

#include "Setup.hpp"
#include "Core/Game.hpp"
#include "Core/Window.hpp"


namespace game
{
OpenGLRenderer::OpenGLRenderer() noexcept
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void OpenGLRenderer::Init(Game &game) noexcept
{
  game_ = &game;
  context_ = SDL_GL_CreateContext(game_->GetWindow().GetSDLWindow());
  gladLoadGLLoader(SDL_GL_GetProcAddress);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  assert(("Couldn't initialize opengl context", context_ != nullptr));
}

void OpenGLRenderer::AddToQueue() noexcept
{
}

void OpenGLRenderer::Render() noexcept
{
  static bool flip = true;
  static int buffer = 1000;
  buffer--;
  if(buffer < 0)
  {
    flip = !flip;
    buffer = 1000;
    if(flip)
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    else
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  }

  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(game_->GetWindow().GetSDLWindow());
}

void OpenGLRenderer::Exit() noexcept
{
  SDL_GL_DeleteContext(context_);
}
} // game