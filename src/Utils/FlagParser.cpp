#include "FlagParser.hpp"

#include <string>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "Setup.hpp"
#include "Utils/String.hpp"


namespace game
{
auto Flags::Count(const std::string &flag, const char **first, const char **last) noexcept -> std::size_t
{
  std::size_t count = 0;

  for(; first != last; first++)
    count += StringStartsWith(flag, std::string(*first));

  return count;
}

auto Flags::Contains(const std::string &flag, const char **first, const char **last) noexcept -> const char **
{
  for(; first != last; first++)
  {
    if(StringStartsWith(flag, std::string(*first)))
      return first;
  }

  return nullptr;
}

auto Flags::Get(const std::string &flag, const char **first, const char **last) noexcept -> std::string
{
  std::size_t strings_used = 0;
  for(; first != last; first++)
  {
    const auto &key_and_value = ParseFlag(first, last, &strings_used);
    if(key_and_value.first == flag)
      return key_and_value.second;
    
    first += strings_used;
  }

  return "";
}

auto Flags::ParseFlag(const char **flag_begin, const char **possible_flag_end, std::size_t *strings_used, const std::string &delim) noexcept -> MapType::value_type
{
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
  if(strings_used != nullptr)
    *strings_used = 1;

  int state = kReadingKey;
  int in_state = 0;
  std::size_t flag_size = strlen(*flag_begin);
  std::string key;
  std::string value;

  std::size_t i = 0;
  while(true)
  {
    switch(state)
    {
    case kReadingKey:
      if((*flag_begin)[i] == '\0')
      {
        if(in_state == 0)
        {
          // Parse error
          std::cout << "Flag parsing error: expected key, got null terminator in flag " << *flag_begin << '\n';
          return MapType::value_type();
        }

        return std::make_pair(flag_begin[0], "");
      }

      if((*flag_begin)[i] == delim[0])
      {
        if(in_state == 0)
        {
          // Parse error
          std::cout << "Flag parsing error: expected key name before value delimetr in flag" << *flag_begin << '\n';
          return MapType::value_type();
        }

        key = std::string((*flag_begin), in_state);
        
        in_state = 0;
        state = kReadingDelimetr;
        i--;
        break;
      }

      in_state++;
      break;
    case kReadingDelimetr:
      if((*flag_begin)[i] == '\0')
      {
        // Parse error
        std::cout << "Flag parsing error: expected delimetr, got null terminator in flag " << *flag_begin << '\n';
        return MapType::value_type();
      }

      if(static_cast<std::size_t>(in_state) == delim.size())
      {
        in_state = 0;
        state = kValueCrossroad;
        i--;
        break;
      }

      if((*flag_begin)[i] != delim[in_state])
      {
        // Parse error
        std::cout << "Flag parsing error: invalid delimetr. Expected " << in_state << ' ' << delim << " got " << std::string(&(*flag_begin)[i - in_state], in_state + 1) << " in flag " << *flag_begin << '\n';
        return MapType::value_type();
      }

      in_state++;
      break;
    case kValueCrossroad:
      if((*flag_begin)[i] == '"')
      {
        state = kQuotedValue;
        break;
      }
      else if((*flag_begin)[i] == '\0') // it stays under, because other variants are more likely
      {
        // Parse error
        std::cout << "Flag parsing error: expected value, got null terminator in flag" << *flag_begin << '\n';
        return MapType::value_type();
      }

      state = kUnquotedValue;
      i--; // unquoted value needs to know about current char
      break;
    case kQuotedValue:
      switch((*flag_begin)[i])
      {
      case '\0':
        value += std::string((*flag_begin) + flag_size - in_state, in_state) + " ";

        flag_begin++;
        if(flag_begin >= possible_flag_end)
        {
          // Parse error
          std::cout << "Flag parsing error: expected quotation mark, got null terminator in flag" << *(flag_begin - 1) << '\n';
          return MapType::value_type();
        }

        flag_size = strlen(*flag_begin);
        if(strings_used != nullptr)
          *strings_used += 1;

        i = 0;
        in_state = 0;
        continue;
      case '\\':
        value += std::string((*flag_begin) + i - in_state, in_state);
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
      if((*flag_begin)[i] == '\0')
      {
        value += std::string((*flag_begin) + flag_size - in_state, in_state) + " ";

        flag_begin++;
        if(flag_begin >= possible_flag_end)
        {
          // Parse error
          std::cout << "Flag parsing error: expected quotation mark, got null terminator in flag " << *(flag_begin - 1) << '\n';
          return MapType::value_type();
        }

        flag_size = strlen(*flag_begin);
        if(strings_used != nullptr)
          *strings_used += 1;

        i = 0;
        continue;
      }

      state = kQuotedValue;
      break;
    case kEndOfQuote:
      if((*flag_begin)[i] == '\0')
        return std::make_pair(key, value);
      
      // Parse error
      std::cout << "Flag parsing error: expected null terminator after quotation mark, got " << (*flag_begin)[i] << " in flag " << *flag_begin << '\n';
      return MapType::value_type();
    case kUnquotedValue:
      return std::make_pair(key, std::string(&(*flag_begin)[i]));
    }

    i++;
  }
}

void Flags::Parse() noexcept
{
  std::size_t strings_used = 0;
  for(const char **str = begin_; str != end_;)
  {
    flags_.insert(ParseFlag(str, end_, &strings_used, delim_));
    str += strings_used;
  }
}
} // game