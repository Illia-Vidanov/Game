#ifndef FLAG_PARSER_HPP
#define FLAG_PARSER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>

#include "Setup.hpp"
#include "Utils/String.hpp"


namespace game
{
/// @brief Class to parse flags
///
/// Values being supported in format of (flag)(delimetr)(optional quote)(value even with space)(optional quote)
class Flags
{
public:
  using MapType = std::unordered_multimap<std::string, std::string>;
  using RangeType = std::pair<MapType::const_iterator, MapType::const_iterator>;

  /// @brief Parse flags with delimetr "="
  ///
  /// @param argc argument count
  /// @param argv pointer to array of c-style strings(arguments)
  inline Flags(const int argc, const char **argv) noexcept : begin_(argv), end_(argv + argc) { Parse(); }

  /// @brief Parse flags with custom delimetr
  ///
  /// @param argc argument count
  /// @param argv pointer to array of c-style strings(arguments)
  /// @param flag_delim delimetr between flag and value (Example: -flag=value  '=' is flag_delim here)
  inline Flags(const int argc, const char **argv, const std::string &flag_delim) noexcept : begin_(argv), end_(argv + argc), delim_(flag_delim) { assert(!flag_delim.empty()); Parse(); }


  /// @brief Count of flags
  ///
  /// @param flag
  /// @return count of same @param flags
  inline auto Count(const std::string &flag) const noexcept -> int { return flags_.count(flag); }

  /// @brief Check if contains a @param flag
  ///
  /// @param flag
  /// @return true if args contain @param flag , false otherwise
  inline auto Contains(const std::string &flag) const noexcept -> bool { return flags_.find(flag) != flags_.end(); }

  /// @brief Get flag value
  ///
  /// If flag doesn't contain a value or don't exists empty string is returned
  /// @param flag
  /// @return string with flag value, if multiple are present any of them can be returned
  inline auto Get(const std::string &flag) const noexcept -> std::string { const auto it = flags_.find(flag); return (it == flags_.end() ? "" : it->second); }
  /// Get range of flags with same key
  ///
  /// If flag doesn't exist past the end element is returned for both iterators
  /// @param flag
  /// @return std::pair<MapType::const_iterator, MapType::const_iterator>(begin, end) i.e. Flags::RangeType
  inline auto GetRange(const std::string &flag) const noexcept -> RangeType { return flags_.equal_range(flag); }


  /// @brief Get constant iterator to the begining of flag map
  ///
  /// @return constant iterator to the begining of flag map
  inline auto begin() const noexcept -> MapType::const_iterator { return flags_.cbegin(); }

  /// @brief  Get constant iterator to the end of flag map
  ///
  /// @return constant iterator to the end of flag map
  inline auto end() const noexcept -> MapType::const_iterator { return flags_.cend(); }


  /// @brief Concate arguments to a single string
  ///
  /// @param delim (default - " ")
  /// @return A string containing all flags separated by @param delim
  inline auto GetArgsAsString(const std::string &delim = " ") const noexcept -> std::string { return ConcatStringArray(begin_, end_, delim); }


  /// @brief Count of flags
  ///
  /// @param flag
  /// @param first first element of arguments array
  /// @param last past the end element of arguments array
  /// @return count of same flags
  static auto Count(const std::string &flag, const char **first, const char **last) noexcept -> std::size_t;

  /// @brief Check if contains a @param flag
  ///
  /// @param flag
  /// @param first first element of arguments array
  /// @param last past the end element of arguments array
  /// @return true if args contain such flag, false otherwise
  static auto Contains(const std::string &flag, const char **first, const char **last) noexcept -> const char**;

  /// @brief Get flag value
  ///
  /// If flag doesn't contain a value or don't exists empty string is returned
  /// @param flag
  /// @param first first element of arguments array
  /// @param last past the end element of arguments array
  /// @return string with flag value, if multiple are present any of them can be returned
  static auto Get(const std::string &flag, const char **first, const char **last) noexcept -> std::string;

  /// @brief Default delimetr between flag and value
  static inline constexpr const char *kDefDelim = "=";

private:
  const char **begin_;
  const char **end_;

  const std::string delim_ = kDefDelim;

  MapType flags_;

  /// @brief Parse flag into key/value pair
  ///
  /// @param flag_begin first falg to be parsed
  /// @param possible_flag_end past the end element of the whole flags array. If flag might be made of several strings, spaces inside value, uses more than one flag
  /// @param[out] strings_used (optional) ammount of strings used by function, minimum 1
  /// @param delim (default - " ") delimetr between flag and value (Example: -flag=value '=' is flag_delim here)
  /// @return std::pair<std::string, std::string>(flag, value)
  static auto ParseFlag(const char **flag_begin, const char **possible_flag_end, std::size_t *strings_used = nullptr, const std::string &delim = kDefDelim) noexcept -> MapType::value_type;
  
  /// @brief Parse internal flags
  ///
  /// Polutes flags_ map with values
  void Parse() noexcept;
};
} // game

#endif // FLAG_PARSER_HPP