#include "FlagParser.hpp"

#include "Setup.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "Utils/String.hpp"


namespace game
{
auto Flags::ParseFlag(ConstArgvType flag_begin, const ConstArgvType possible_flag_end, std::size_t *strings_used, const std::string &delim) noexcept -> MapType::value_type
{
  ZoneScopedC(0xbaed00);

  enum
  {
    kReadingKey,
    kReadingDelimetr,
    kValueCrossroad,
    kQuotedValue,
    kInternalQuotationMark,
    kEndOfQuote,
    kUnquotedValue
  };

  if(delim.empty())
    return std::make_pair(flag_begin[0], "");

  // always at least one is used
  *strings_used = 1;
  int total_strings_used = 0;

  int state = kReadingKey;
  int in_state = 0;
  std::size_t flag_size = std::char_traits<char>::length(*flag_begin);
  std::string key;
  std::string value;

  std::size_t str_index = 0;
  while(true)
  {
    switch(state)
    {
    case kReadingKey:
      if((*flag_begin)[str_index] == '\0')
      {
        if(in_state == 0)
        {
          // Parse error
          GAME_LOG(LogType::Warning) << "Flag parsing error: expected key, got null terminator in flag " << *flag_begin << '\n';
          return MapType::value_type();
        }

        return std::make_pair(flag_begin[0], "");
      }

      if((*flag_begin)[str_index] == delim[0])
      {
        if(in_state == 0)
        {
          // Parse error
          GAME_LOG(LogType::Warning) << "Flag parsing error: expected key name before value delimetr in flag" << *flag_begin << '\n';
          return MapType::value_type();
        }

        key = std::string((*flag_begin), in_state);
        
        in_state = 0;
        state = kReadingDelimetr;
        str_index--;
        break;
      }

      in_state++;
      break;
    case kReadingDelimetr:
      if((*flag_begin)[str_index] == '\0')
      {
        // Parse error
        GAME_LOG(LogType::Warning) << "Flag parsing error: expected delimetr, got null terminator in flag " << *flag_begin << '\n';
        return MapType::value_type();
      }

      if(static_cast<std::size_t>(in_state) == delim.size())
      {
        in_state = 0;
        state = kValueCrossroad;
        str_index--;
        break;
      }

      if((*flag_begin)[str_index] != delim[in_state])
      {
        // Parse error
        GAME_LOG(LogType::Warning) << "Flag parsing error: invalid delimetr. Expected " << in_state << ' ' << delim << " got " << std::string(&(*flag_begin)[str_index - in_state], in_state + 1) << " in flag " << *flag_begin << '\n';
        return MapType::value_type();
      }

      in_state++;
      break;
    case kValueCrossroad:
      if((*flag_begin)[str_index] == '"')
      {
        state = kQuotedValue;
        break;
      }
      else if((*flag_begin)[str_index] == '\0') // it stays under, because other variants are more likely
      {
        // Parse error
        GAME_LOG(LogType::Warning) << "Flag parsing error: expected value, got null terminator in flag" << *flag_begin << '\n';
        return MapType::value_type();
      }

      state = kUnquotedValue;
      str_index--; // unquoted value needs to know about current char
      break;
    case kQuotedValue:
      switch((*flag_begin)[str_index])
      {
      case '\0':
        value += std::string((*flag_begin) + flag_size - in_state, in_state) + " ";

        flag_begin++;
        if(flag_begin >= possible_flag_end)
        {
          // Parse error
          GAME_LOG(LogType::Warning) << "Flag parsing error: expected quotation mark, got null terminator in flag" << *(flag_begin - 1) << '\n';
          return MapType::value_type();
        }

        flag_size = std::char_traits<char>::length(*flag_begin);
        total_strings_used += 1;

        str_index = 0;
        in_state = 0;
        continue;
      case '\\':
        value += std::string((*flag_begin) + str_index - in_state, in_state);
        in_state = 0;
        state = kInternalQuotationMark;
        break;
      case '"':
        value += std::string((*flag_begin) + flag_size - in_state - 1, in_state); // - 1 to account null terminator, we expect it to be next, if not parsing error is printed and empty string returned
        state = kEndOfQuote;
        break;
      default:
        break;
      }

      in_state++;
      break;
    case kInternalQuotationMark:
      if((*flag_begin)[str_index] == '\0')
      {
        value += std::string((*flag_begin) + flag_size - in_state, in_state) + " ";

        flag_begin++;
        if(flag_begin >= possible_flag_end)
        {
          // Parse error
          GAME_LOG(LogType::Warning) << "Flag parsing error: expected quotation mark, got null terminator in flag " << *(flag_begin - 1) << '\n';
          return MapType::value_type();
        }

        flag_size = std::char_traits<char>::length(*flag_begin);
        total_strings_used += 1;

        str_index = 0;
        continue;
      }

      state = kQuotedValue;
      break;
    case kEndOfQuote:
      if(strings_used)
        *strings_used += total_strings_used;
      if((*flag_begin)[str_index] == '\0')
        return std::make_pair(key, value);
      
      // Parse error
      GAME_LOG(LogType::Warning) << "Flag parsing error: expected null terminator after quotation mark, got " << (*flag_begin)[str_index] << " in flag " << *flag_begin << '\n';
      return MapType::value_type();
    case kUnquotedValue:
      if(strings_used)
        *strings_used += total_strings_used;
      return std::make_pair(key, std::string(&(*flag_begin)[str_index]));
    }

    str_index++;
  }
}

void Flags::Parse() noexcept
{
  ZoneScopedC(0xbaed00);
  
  std::size_t strings_used = 0;
  for(ConstArgvType str = begin_; str != end_;)
  {
    const MapType::value_type &val = ParseFlag(str, end_, &strings_used, delim_);
    if(!val.first.empty())
      flags_.insert(val);
    str += strings_used;
  }
}
} // game