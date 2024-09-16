#ifndef GAME_EVENT_HANDLER_HPP
#define GAME_EVENT_HANDLER_HPP

#include "Setup.hpp"

#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <string>
#include <type_traits>

#include "Utils/Logger.hpp"


namespace game
{
enum class EventType : uint32_t
{
  None = 0,
  KeyDown,
  KeyUp,
  KeyPressed,
  Quit
};

union Event
{
public:
  static constexpr inline std::underlying_type_t<EventType> kCustomTypeBitMask = std::underlying_type_t<EventType>{1} << (sizeof(EventType) * CHAR_BIT - 1);

  
  [[nodiscard]] constexpr inline auto GetType() const noexcept -> EventType { return common.type; }
  /// Get name of event according to type
  /// If type is custom "Other: (type value)" is returned
  /// Mainly debuging feature
  [[nodiscard]] auto GetName() const noexcept -> std::string;
	
	[[nodiscard]] inline auto GetKeycode() const noexcept -> uint16_t
  { GAME_ASSERT_STD(common.type == EventType::KeyDown || common.type == EventType::KeyUp || common.type == EventType::KeyPressed, "Acces data from wrong event type. Expected: kKeyDown/kKeyUp/kKeyPressed"); return keyboard.keycode; }
	[[nodiscard]] inline auto GetScancode() const noexcept -> uint16_t
  { GAME_ASSERT_STD(common.type == EventType::KeyDown || common.type == EventType::KeyUp || common.type == EventType::KeyPressed, "Acces data from wrong event type. Expected: kKeyDown/kKeyUp/kKeyPressed"); return keyboard.scancode; }
	[[nodiscard]] inline auto GetModKeys() const noexcept -> uint16_t
  { GAME_ASSERT_STD(common.type == EventType::KeyDown || common.type == EventType::KeyUp || common.type == EventType::KeyPressed, "Acces data from wrong event type. Expected: kKeyDown/kKeyUp/kKeyPressed"); return keyboard.mod_keys; }
	[[nodiscard]] inline auto GetCustomData() const noexcept -> void*
  { GAME_ASSERT_STD(common.type & kCustomTypeBitMask, "Acces data from wrong event type. Expected: to contain kCustomTypeBitMask"); return custom.data; }


  struct Common
  {
    friend Event;
  public:
    explicit constexpr inline Common(EventType type_data) noexcept : type(type_data) {}

  // private here is used as const to make it immutable by users
  private:
    EventType type;
  };
	

  struct Keyboard
  {
    constexpr inline Keyboard(uint16_t keycode_data, uint16_t scancode_data, uint16_t mod_keys_data) noexcept : keycode{keycode_data}, scancode{scancode_data}, mod_keys{mod_keys_data} {}
    uint16_t keycode;
		uint16_t scancode;
		uint16_t mod_keys;
    uint16_t padding = 0;
  };


	struct KeyDown : Common, Keyboard
	{
    explicit constexpr inline KeyDown(const Keyboard &keyboard_data) noexcept : Common{EventType::KeyDown}, Keyboard{keyboard_data} {}
	};
	constexpr inline Event(const KeyDown &key_down_data) noexcept : key_down{key_down_data} {}


  struct KeyUp : Common, Keyboard
	{
    explicit constexpr inline KeyUp(const Keyboard &keyboard_data) noexcept : Common{EventType::KeyUp}, Keyboard{keyboard_data} {}
	};
	constexpr inline Event(const KeyUp &key_up_data) noexcept : key_up{key_up_data} {}


  struct KeyPressed : Common, Keyboard
	{
    explicit constexpr inline KeyPressed(const Keyboard &keyboard_data) noexcept : Common{EventType::KeyPressed}, Keyboard{keyboard_data} {}
	};
	constexpr inline Event(const KeyPressed &key_pressed_data) noexcept : key_pressed{key_pressed_data} {}


  struct Quit : Common
	{
		explicit constexpr inline Quit() noexcept : Common(EventType::Quit) {}
	};
	inline Event(const Quit &quit_data) noexcept : quit{quit_data} {}
	

	/// Custom type should specify it's type for listeners and it's last bit should be 1 or contain just use kCustomTypeBitMask
	struct Custom : Common
	{
    explicit constexpr inline Custom(EventType custom_type, void *data_data) noexcept : Common(custom_type), data(data_data)
    { GAME_ASSERT_STD(custom_type & kCustomTypeBitMask, "Custom type's should contain kCustomTypeBitMask"); }
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
  using MapType = std::multimap<EventType, MapValueType>;
  using MapPtrType = MapType::iterator;
  using QueueType = std::queue<Event>;


  /// Add to type event listener that will be trigered when event fires
  /// First in is first to be called when event happens 
  auto AddListener(EventCleaner &cleaner, EventType type, void *data, CallbackType callback) noexcept -> MapPtrType;
  inline void RemoveListener(EventCleaner &cleaner, MapPtrType ptr) noexcept;
  /// Remove all listeners of specified type
  void ClearListeners(EventType type) noexcept;
 
  /// Instantly dispatch event
  void DispatchEvent(const Event &event) noexcept;
  /// Poll and dispatch al current SDL events
  void DispatchSDLEvents() noexcept;
  inline void EnqueEvent(const Event &event) noexcept { queue_.push(event); }
  inline void DispatchEnquedEvents() noexcept { ZoneScopedC(0xe8bb25); while(queue_.size()) { DispatchEvent(queue_.front()); queue_.pop(); } }


private:
  /// Used in event cleaner to directly remove listener when EventCleaner is destroyed
  inline void RemoveListener(MapPtrType ptr) noexcept { listeners_.erase(ptr); }  

  QueueType queue_;
  MapType listeners_;
};



/// Class that manages lifetime of event listeners
/// It removes listeners that were created using it from EventHandler on destruction
class EventCleaner
{
  friend EventHandler;
public:
  inline EventCleaner(EventHandler &events) noexcept : events_{events} {}
  inline ~EventCleaner() noexcept { ZoneScopedC(0xe8bb25); for(auto ptr : ptrs_) events_.RemoveListener(ptr); }

  /// Add ptr (you can get it when adding listener in EventHandler) to EventClener that will be removed from EventHandler on the destruction
  inline void AddPtr(EventHandler::MapPtrType ptr) noexcept { ptrs_.push_back(ptr); }
  /// Remove ptr (you can get it when adding listener in EventHandler) from EventCleaner to not remove it on the destruction of a cleaner
  inline void RemovePtr(EventHandler::MapPtrType ptr) noexcept { std::swap(ptrs_.back(), *std::find(ptrs_.begin(), ptrs_.end(), ptr)); ptrs_.pop_back(); }
  /// Remove all ptrs from cleaner so they won't be removed on destruction of EventCleaner
  inline void ClearPtrs() noexcept { ptrs_.clear(); }

private:
  EventHandler &events_;
  std::vector<EventHandler::MapPtrType> ptrs_;
};



inline void EventHandler::RemoveListener(EventCleaner &cleaner, MapPtrType ptr) noexcept { listeners_.erase(ptr); cleaner.RemovePtr(ptr); }
} // game

#endif // GAME_EVENT_HANDLER_HPP