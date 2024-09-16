#include "EventHandler.hpp"

#include "Setup.hpp"

#include <utility>

#include <SDL2/SDL.h>


namespace game
{
void EventHandler::DispatchEvent(const Event &event) noexcept
{
  ZoneScopedC(0xe8bb25);

  ZoneText(event.GetName().c_str(), event.GetName().size());

  std::pair<MapType::const_iterator, MapType::const_iterator> range = listeners_.equal_range(event.GetType());

  ZoneValue(std::distance(range.first, range.second));

  for(; range.first != range.second; ++range.first)
  {
    // range.first = current listener
    // range.first->second = value of listener
    // range.first->second.first = data
    // range.first->second.second = callback
    if(!range.first->second.second(event, range.first->second.first))
      break;
  }
}

void EventHandler::ClearListeners(Event::TypeType type) noexcept
{
  std::pair<MapType::const_iterator, MapType::const_iterator> range = listeners_.equal_range(type);
  listeners_.erase(range.first, range.second);
}

auto EventHandler::AddListener(EventCleaner &cleaner, Event::TypeType type, void *data, CallbackType callback) noexcept -> MapPtrType
{
  MapPtrType it = listeners_.insert(MapType::value_type(type, MapValueType(data, callback)));
  cleaner.AddPtr(it);
  return it;
}

void EventHandler::DispatchSDLEvents() noexcept
{
  ZoneScopedC(0xe8bb25);

  SDL_Event event;
  #ifndef TRACY_ENABLE
  while(SDL_PollEvent(&event))
  {
  #else
  while(true)
  {
    {
      ZoneScopedNC("Poll SDL event", 0xe8bb25);
      int state = SDL_PollEvent(&event);
      if(!state)
        break;
      ZoneValue(event.type);
    }
  #endif
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

auto Event::GetName() const noexcept -> std::string
{
  switch(GetType())
  {
  case kNoneEvent:
    return "None";
  case kQuitEvent:
    return "Quit";
  case kKeyDownEvent:
    return "Key Down";
  case kKeyUpEvent:
    return "Key Up";
  case kKeyPressedEvent:
    return "Key Pressed";
  default:
    return "Other: " + std::to_string(GetType());
  }
}
}