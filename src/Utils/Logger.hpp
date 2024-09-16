#ifndef GAME_LOGGER_HPP
#define GAME_LOGGER_HPP

#include "Setup.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <filesystem>
#include <cassert>

#include "Utils/Enum.hpp"

/* Todo:
*  Show line, file, condition, time, error type
*  Color
*  Formatter
*  Setters
*/

#if defined(NDEBUG) && !defined(TRACY_ENABLE)
  #define GAME_DLOG(type) if constexpr (false) game::LogStream(LogData{__LINE__, __FILE__, LogType::None})
  #define GAME_VDLOG(level, type) if constexpr (false) game::LogStream(LogData{__LINE__, __FILE__, LogType::None})
  #define GAME_ASSERT(condition) if constexpr (false) game::LogStream(LogData{__LINE__, __FILE__, LogType::None})
  #define GAME_ASSERT_STD(condition, message)
#else
  /// Log message only in when NDEBUG is 0
  #define GAME_DLOG(type) GAME_LOG(type)
  /// Log message only in when NDEBUG is 0 and verbose level is smaller than current Logger's verbose level
  #define GAME_VDLOG(level, type) GAME_VLOG(level, type)
  /// Assert if condition is false
  #define GAME_ASSERT(condition) if(GAME_IS_UNLIKELY(!(!!(condition)))) GAME_LOG(LogType::Fatal)
  #define GAME_ASSERT_STD(condition, message) assert(((void)(message), (condition)))
#endif

#define GAME_LOG(type) game::LogStream(LogData{__LINE__, __FILE__, type})
#define GAME_VLOG(level, type) if(level < game::Logger::Get().GetVerboseLevel()) GAME_LOG(type)

namespace game
{
enum class LogType : uint8_t
{
  None, Fatal, Error, Warning, Info
};


class Logger
{
  friend class LogStream;
public:
	using StreamCharT = char;
  using VerboseLevelT = int;

  /// Global instance of logger
  [[nodiscard]] static inline auto Get() -> Logger& { static Logger instance; return instance; }

  [[nodiscard]] constexpr inline auto GetStream() noexcept -> std::basic_ostream<StreamCharT>& { return stream_; }
  [[nodiscard]] constexpr inline auto GetFile() noexcept -> std::basic_ofstream<StreamCharT>& { return file_; }
  [[nodiscard]] constexpr inline auto IsOutputToFile() const noexcept -> bool { return output_to_file_; }
  [[nodiscard]] constexpr inline auto GetVerboseLevel() const noexcept -> int { return verbose_level_; }

  /// Sets verbose levels such that values with heigher level won't be printed
  constexpr inline void SetVerboseLevel(VerboseLevelT verbose_level) noexcept { verbose_level_ = verbose_level; }
  /// If true output is also writen to file
  /// Default value is true 
  constexpr inline void SetOutputToFile(bool output_to_file) noexcept { output_to_file_ = output_to_file; }
  /// Set buffer of underlying stream
  /// Set to:
  /// std::cerr.rdbuf() to use std::cerr
  /// std::cout.rdbuf() to use std::cout
  /// etc.
  inline void SetOutStreamBuffer(std::basic_ostream<StreamCharT> &stream) noexcept { stream_.rdbuf(stream.rdbuf()); }

  /// Get string with name of LogType
  [[nodiscard]] static inline auto GetLogTypeName(LogType type) noexcept -> std::string { static const std::string kLogTypeNameMap[] = { "Fatal", "Error", "Warning", "Info" }; return kLogTypeNameMap[ToUnderlying(type)]; }

  static inline const std::filesystem::path kDefLogPath{"./logs/"};
  static inline const std::string kDefLogFileName{"last_log.txt"};
  static inline const std::basic_ostream<StreamCharT> kDefStream{std::cout.rdbuf()};
  static constexpr inline VerboseLevelT kDefVerboseLevel{0};

private:
  Logger() noexcept;
  ~Logger() noexcept;

  std::basic_ostream<StreamCharT> stream_{kDefStream.rdbuf()};
  bool output_to_file_ = true;
  VerboseLevelT verbose_level_{0};
  std::string file_path_{kDefLogPath.generic_u8string() + kDefLogFileName};
	std::basic_ofstream<StreamCharT> file_;
};


struct LogData
{
  int line;
  const char *file;
  LogType type;
};


/// Stream that uses global information from Logger to log
class LogStream
{
public:
  LogStream(const LogData &log_data) noexcept : fatal_(log_data.type == LogType::Fatal) { GAME_ASSERT_STD(log_data.type != LogType::None, "LogType::None"); }
  ~LogStream() noexcept;
	
  /// Input value to be printed
  template<typename T>
	inline LogStream &operator<<(T &&t) noexcept;

private:
  bool fatal_ = false;
	
  static inline std::basic_stringstream<Logger::StreamCharT> string_stream;
};

template<typename T>
inline LogStream &LogStream::operator<<(T &&t) noexcept
{
  ZoneScopedNC("Log vaue", 0xbaed00);
  
  string_stream << std::forward<T>(t);
	return *this;
}

namespace detail
{
class Vodify
{
public:
  constexpr inline void operator&(__attribute__((unused)) const LogStream &) const noexcept {}
};
} // detail
} // game

#endif // GAME_LOGGER_HPP