cmake_minimum_required(VERSION 3.12)
project(
  Game
  VERSION 0
)

# Setters
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glad)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
)

link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/gcem/include
)

link_libraries(
  glad
  mingw32
  SDL2
  SDL2main
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wdeprecated -Wshadow -pedantic-errors -fmax-errors=3")

# Game
add_executable(
  ${CMAKE_PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Game.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/EventHandler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Window.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Renderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/FlagParser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/OpenGL/OpenGLRenderer.cpp
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})