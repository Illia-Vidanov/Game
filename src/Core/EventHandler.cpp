#include "EventHandler.hpp"

#include "Setup.hpp"

#include <utility>

#include <SDL2/SDL.h>


namespace game
{
void EventHandler::DispatchEvent(const Event &event) noexcept
{
  std::pair<MapConstIterType, MapConstIterType> range = listeners_.equal_range(event.GetType());
  MapConstIterType rend = --range.first; 
  for(--range.second; range.second != rend; --range.second)
  {
    // range.second = current listener
    // range.second->second = value of listener
    // range.second->second.first = data
    // range.second->second.second = callback
    if(!range.second->second.second(event, range.second->second.first))
      break;
  }
}

auto EventHandler::AddListener(EventCleaner &cleaner, Event::TypeType type, void *data, CallbackType callback) noexcept -> MapConstIterType
{
  MapConstIterType it = listeners_.insert(MapType::value_type(type, MapValueType(data, callback)));
  cleaner.AddIter(it);
  return it;
}

void EventHandler::DispatchSDLEvents() noexcept
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
    case SDL_QUIT:
      DispatchEvent(Event{Event::Quit{}});
      break;
    case SDL_KEYDOWN:
      DispatchEvent(Event{Event::KeyDown{Event::Keyboard{
        static_cast<uint16_t>(event.key.keysym.sym),
        static_cast<uint16_t>(event.key.keysym.scancode),
        static_cast<uint16_t>(event.key.keysym.mod)
      }}});
      break;
    default:
      break;
    }
  }
}
}