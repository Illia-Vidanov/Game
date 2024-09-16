#ifndef FLAG_PARSER_HPP
#define FLAG_PARSER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>

#include "Setup.hpp"
#include "Utils/String.hpp"
#include "Utils/Logger.hpp"


namespace game
{
/// Class to parse flags
///
/// Values being supported in format of (flag)(delimetr)(value)
/// Value can be with spaces if surrounded by quotes '"'
/// Can use quotes inside quotes if back slash '\' is present before quotation mark
class Flags
{
public:
  using MapType = std::unordered_multimap<std::string, std::string>;
  using RangeType = std::pair<MapType::const_iterator, MapType::const_iterator>;
  using ConstArgvType = const char * const *;

  /// Initialize class with arguments from main function
  inline Flags(const int argc, ConstArgvType argv) noexcept : begin_(argv), end_(argv + argc) { Parse(); }

  /// Parse flags with custom delimetr
  /// default flag_delim = "="
  /// It's delimetr between flag and value (Example: -flag=value   "=" is flag_delim here)
  inline Flags(const int argc, ConstArgvType argv, const std::string &flag_delim) noexcept : begin_(argv), end_(argv + argc), delim_(flag_delim) { GAME_ASSERT(!flag_delim.empty()); Parse(); }


  /// Count of flags
  /// return occurrences of same flag
  inline auto Count(const std::string &flag) const noexcept -> int { return flags_.count(flag); }

  /// Check if contains a @param flag
  /// return true if contains flag, false otherwise
  inline auto Contains(const std::string &flag) const noexcept -> bool { return flags_.find(flag) != flags_.end(); }

  /// Get flag value
  /// If flag doesn't contain a value or don't exists empty string is returned
  /// return string with flag value, if multiple are present any of them can be returned
  inline auto Get(const std::string &flag) const noexcept -> std::string { const auto it = flags_.find(flag); return (it == flags_.end() ? "" : it->second); }
  
  /// Get range of flags with same key
  /// If flag doesn't exist past the end element is returned for both iterators
  /// return pair with begin and end of range i.e. Flags::RangeType
  inline auto GetRange(const std::string &flag) const noexcept -> RangeType { return flags_.equal_range(flag); }


  /// Get constant iterator to the begining of flag map
  inline auto begin() const noexcept -> MapType::const_iterator { return flags_.cbegin(); }

  /// Get constant iterator to the end of flag map
  inline auto end() const noexcept -> MapType::const_iterator { return flags_.cend(); }


  /// Concate arguments to a single string
  /// default delim = " "
  /// return a string containing all flags separated by delim
  inline auto GetArgsAsString(const std::string &delim = " ") const noexcept -> std::string { return ConcatStringArray(begin_, end_, delim); }


  /// Default delimetr between flag and value
  static constexpr inline const char *kDefDelim = "=";

private:
  const ConstArgvType begin_;
  const ConstArgvType end_;
  const std::string delim_ = kDefDelim;

  MapType flags_;

  /// Parse flag into key/value pair
  /// flag_begin first falg to be parsed
  /// possible_flag_end past the end element of the whole flags array. If flag might be made of several strings, spaces inside value, uses more than one flag
  /// strings_used (out, optional) ammount of strings used by function, minimum 1
  /// delimetr (default = " ") between flag and value (Example: -flag=value    '=' is flag_delim here)
  /// return pair with flag and value
  static auto ParseFlag(ConstArgvType flag_begin, const ConstArgvType possible_flag_end, std::size_t *strings_used = nullptr, const std::string &delim = kDefDelim) noexcept -> MapType::value_type;
  
  /// Parse cmd flags
  void Parse() noexcept;
};
} // game

#endif // FLAG_PARSER_HPP