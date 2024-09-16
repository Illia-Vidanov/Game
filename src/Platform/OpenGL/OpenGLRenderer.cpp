#include "OpenGLRenderer.hpp"

#include "glad/glad.h"
#include "SDL2/SDL.h"

#include "Setup.hpp"
#include "Core/Game.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"


#ifndef NDEBUG
#define GL_CALL(call) do { \
    call; \
    while(int error = glGetError()) \
      GAME_DLOG(LogType::Error) << "OpenGL error: " << error; \
  } while(0)
#else
#define GL_CALL(call) call
#endif

namespace game
{
OpenGLRenderer::OpenGLRenderer() noexcept
{
  ZoneScopedC(0x07dbd4);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void OpenGLRenderer::Init(Game &game) noexcept
{
  ZoneScopedC(0x07dbd4);

  game_ = &game;
  context_ = SDL_GL_CreateContext(game_->GetWindow().GetSDLWindow());
  GAME_ASSERT(context_ != nullptr) << "Couldn't initialize opengl context: " << SDL_GetError();
  if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    GAME_ASSERT(false) << "Failed to load GL loader";

  GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void OpenGLRenderer::Render() noexcept
{
  ZoneScopedC(0x07dbd4);

  static bool flip = true;
  static int buffer = 1000;
  buffer--;
  if(buffer < 0)
  {
    flip = !flip;
    buffer = 1000;
    if(flip)
      GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    else
      GL_CALL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
  }

  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
  SDL_GL_SwapWindow(game_->GetWindow().GetSDLWindow());
}

void OpenGLRenderer::Exit() noexcept
{
  ZoneScopedC(0x07dbd4);

  SDL_GL_DeleteContext(context_);
}
} // game