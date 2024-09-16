#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <filesystem>

#include "Setup.hpp"


namespace game
{
namespace detail
{
  template<typename CharT = char>
  using ConstRawStrArray = const CharT * const *;
}

/// Add current path to string
/// return current path + '/' + str
template<typename CharT>
inline auto AddCurrentPathToString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>
{ return std::filesystem::current_path().generic_string<CharT>() + std::basic_string<CharT>(1, static_cast<CharT>('/')) + str; }

template<typename CharT>
inline auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>;

/// Split string by delimetr in vector of strings
/// default delim = " "
template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim = " ") noexcept -> std::vector<std::basic_string<CharT>>;
/// Split string by delimetr in vector of strings
/// default delim = " "
template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, CharT delim) noexcept -> std::vector<std::basic_string<CharT>>
{ return SplitString(str, std::basic_string<CharT>(1, delim)); }

/// Check if string starts with certain prefix
/// Doesn't perform bounds checking so if prefix.size() > string.size() behaviour is undefined
template<typename CharT>
inline auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) noexcept -> bool;

template<typename CharT>
inline auto SubstringCount(const std::basic_string<CharT> &str, const std::basic_string<CharT> &substr) noexcept -> std::size_t;

/// Get charachter count of all strings in array
template<typename CharT>
constexpr inline auto RawStringArrayLength(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last) noexcept -> std::size_t; 

/// Connect raw string array into single string with delimetr
/// default delim = ""
/// return string with elements from first to last separated by delim 
template<typename CharT>
inline auto ConcatStringArray(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last, const std::basic_string<CharT> &delim = "") noexcept -> std::basic_string<CharT>;



template<typename CharT>
inline auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>
{
  ZoneScopedC(0x3e2ed1);

  const std::basic_string<CharT> &path = std::filesystem::current_path().generic_string<CharT>();
  if(!StringStartsWith(path, str))
    return str;
  return std::basic_string<CharT>(str).replace(0, path.size() + 1, ""); // + 1 for slash
}

template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim) noexcept -> std::vector<std::basic_string<CharT>>
{
  ZoneScopedC(0x3e2ed1);

	std::vector<std::basic_string<CharT>> result(SubstringCount(str, delim) + 1);
	
	std::size_t index = 0;
	std::size_t fromPos = 0;
	for(std::size_t delimPos = str.find(delim); delimPos != std::basic_string<CharT>::npos; delimPos = str.find(delim, delimPos + 1))
	{
		result[index] = str.substr(fromPos, delimPos - fromPos);
		if(!result[index].empty())
			index++;
		fromPos = delimPos + delim.size();
	}
	
	result[index] = str.substr(fromPos);
	if(!result[index].empty())
		index++;

  result.resize(index);
  // In some cases we allocate much more memory than we actuall use, but usually this line can be commented out, depends on demands
  //result.shrink_to_fit();

	return result;
}

template<typename CharT>
inline auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) noexcept -> bool
{
  ZoneScopedC(0x3e2ed1);

    for(std::size_t i = 0; i < prefix.size(); i++)
    {
      if(str[i] != prefix[i])
        return false;
    }

    return true;
}

template<typename CharT>
inline auto SubstringCount(const std::basic_string<CharT> &str, const std::basic_string<CharT> &substr) noexcept -> std::size_t
{
  ZoneScopedC(0x3e2ed1);

	std::size_t count = 0;
	std::size_t substrPos = str.find(substr);
  while(substrPos != std::basic_string<CharT>::npos)
	{
		count++;
		substrPos = str.find(substr, substrPos + 1);
	}

	return count;
}

template<typename CharT>
constexpr inline auto RawStringArrayLength(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last) noexcept -> std::size_t
{
  std::size_t result = 0;
	for(; first != last; first++)
		result += std::char_traits<CharT>::length(*first);

	return result;
}

template<typename CharT>
inline auto ConcatStringArray(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last, const std::basic_string<CharT> &delim) noexcept -> std::basic_string<CharT>
{
  ZoneScopedC(0x3e2ed1);

	std::string result;
	result.reserve(RawStringArrayLength(first, last) + (last - first - 1) * delim.size());

  for(; first != last; first++)
  {
		result += *first;
		result += delim;
	}

	return result;
}
} // game

#endif // STRING_UTILS_HPP