#ifndef GAME_EVENT_HANDLER_HPP
#define GAME_EVENT_HANDLER_HPP

#include "Setup.hpp"

#include <map>
#include <functional>
#include <cassert>
#include <queue>
#include <algorithm>


namespace game
{
enum
{
  kKeyDownEvent,
  kKeyUpEvent,
  kKeyPressedEvent,
  kQuitEvent
};

union Event
{
public:
  using TypeType = uint32_t;
  static constexpr inline TypeType kCustomTypeBitMask = TypeType(1) << (sizeof(TypeType) * CHAR_BIT - 1);

  [[nodiscard]] constexpr inline auto GetType() const -> TypeType { return common.type; }
	
	[[nodiscard]] constexpr inline auto GetKeycode() const -> uint16_t { assert(((void)"Acces data from wrong event type", common.type == kKeyDownEvent)); return keyboard.keycode; }
	[[nodiscard]] constexpr inline auto GetScancode() const -> uint16_t { assert(((void)"Acces data from wrong event type", common.type == kKeyDownEvent)); return keyboard.scancode; }
	[[nodiscard]] constexpr inline auto GetModKeys() const -> uint16_t { assert(((void)"Acces data from wrong event type", common.type == kKeyDownEvent)); return keyboard.mod_keys; }
	[[nodiscard]] constexpr inline auto GetCustomData() const -> void* { assert(((void)"Acces data from wrong event type", common.type & kCustomTypeBitMask)); return custom.data; }


  struct Common
  {
    friend Event;
  public:
    explicit constexpr inline Common(TypeType type_data) : type(type_data) {}

  // private here is used as const to make it immutable by users
  private:
    TypeType type;
  };
	

  struct Keyboard
  {
    constexpr inline Keyboard(uint16_t keycode_data, uint16_t scancode_data, uint16_t mod_keys_data) : keycode{keycode_data}, scancode{scancode_data}, mod_keys{mod_keys_data} {}
    uint16_t keycode;
		uint16_t scancode;
		uint16_t mod_keys;
    uint16_t padding = 0;
  };


	struct KeyDown : Common, Keyboard
	{
    explicit constexpr inline KeyDown(const Keyboard &keyboard_data) noexcept : Common{kKeyDownEvent}, Keyboard{keyboard_data} {}
	};
	inline constexpr Event(const KeyDown &key_down_data) noexcept : key_down{key_down_data} {}


  struct KeyUp : Common, Keyboard
	{
    explicit constexpr inline KeyUp(const Keyboard &keyboard_data) noexcept : Common{kKeyUpEvent}, Keyboard{keyboard_data} {}
	};
	inline constexpr Event(const KeyUp &key_up_data) noexcept : key_up{key_up_data} {}


  struct KeyPressed : Common, Keyboard
	{
    explicit constexpr inline KeyPressed(const Keyboard &keyboard_data) noexcept : Common{kKeyPressedEvent}, Keyboard{keyboard_data} {}
	};
	inline constexpr Event(const KeyPressed &key_pressed_data) noexcept : key_pressed{key_pressed_data} {}


  struct Quit : Common
	{
		explicit constexpr inline Quit() noexcept : Common(kQuitEvent) {}
	};
	inline Event(const Quit &quit_data) noexcept : quit{quit_data} {}
	

	/// Custom type should specify it's type for listeners and it's last bit should be 1 or contain just use kCustomTypeBitMask
	struct Custom : Common
	{
    explicit constexpr inline Custom(TypeType custom_type, void *data_data) noexcept : Common(custom_type), data(data_data) { assert(((void)"Custom type's last bit should be 1 or contain just use kCustomTypeBitMask", custom_type & kCustomTypeBitMask)); }
		void *data;
	};
	inline Event(const Custom &custom_data) noexcept : custom{custom_data} {}
  
// private here is used as const to make it immutable by users
private:
  struct CommonKeyboard : Common, Keyboard
  {};

  Common common;
  CommonKeyboard keyboard;
	KeyDown key_down;
	KeyUp key_up;
	KeyPressed key_pressed;
	Quit quit;
	Custom custom;
};



class EventCleaner;

class EventHandler
{
  friend EventCleaner;
public:
  using CallbackType = bool(*)(const Event &, void *data);
  using MapValueType = std::pair<void*, CallbackType>;
  using MapType = std::multimap<Event::TypeType, MapValueType>;
  using MapConstIterType = MapType::const_iterator;
  using QueueType = std::queue<Event>;

  auto AddListener(EventCleaner &cleaner, Event::TypeType type, void *data, CallbackType callback) noexcept -> MapConstIterType;
  inline void RemoveListener(MapConstIterType it) noexcept { listeners_.erase(it); }

  void DispatchEvent(const Event &event) noexcept;
  inline void EnqueEvent(const Event &event) noexcept { queue_.push(event); }
  inline void DispatchEnquedEvents() noexcept { while(queue_.size()) { DispatchEvent(queue_.front()); queue_.pop(); } }

  void DispatchSDLEvents() noexcept;

private:
  QueueType queue_;
  MapType listeners_;
};



class EventCleaner
{
  friend EventHandler;
public:
  inline EventCleaner(EventHandler &events) noexcept : events_{events}, iters_{4} {}
  inline ~EventCleaner() noexcept { for(auto it : iters_) events_.RemoveListener(it); }

  inline void AddIter(EventHandler::MapConstIterType iter) { iters_.push_back(iter); }
  inline void RemoveIter(EventHandler::MapConstIterType iter) { iters_.erase(std::find(iters_.begin(), iters_.end(), iter)); }
  inline void ClearIters() { iters_.clear(); }

private:
  EventHandler &events_;
  std::vector<EventHandler::MapConstIterType> iters_;
};
} // game

#endif // GAME_EVENT_HANDLER_HPP