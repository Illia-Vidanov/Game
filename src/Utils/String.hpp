#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstring>
#include <filesystem>


/// @brief Add current path to string
///
/// @param str
/// @return current path + '/' + @param str
template<typename CharT>
inline auto AddCurrentPathToString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>
{ return std::filesystem::current_path().generic_string<CharT>() + std::basic_string<CharT>(1, static_cast<CharT>('/')) + str; }

/// @brief Remove current path to string
///
/// @param str
/// @return string without current path
template<typename CharT>
inline auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>;

/// @brief Split string by delimetr in vector of strings
///
/// @param str
/// @param delim (default - " ")
/// @return vector of strings
template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim = " ") -> std::vector<std::basic_string<CharT>>;
/// @brief Split string by delimetr in vector of strings
///
/// @param str
/// @param delim (default - " ")
/// @return vector of strings
template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, CharT delim) -> std::vector<std::basic_string<CharT>>
{ return SplitString(str, std::basic_string<CharT>(1, delim)); }

/// @brief Check if string starts with certain prefix
///
/// Doesn't perform bounds checking so if prefix.size() > string.size() behaviour is undefined
/// @param prefix
/// @param str
/// @return true if @param str starts with @param prefix
template<typename CharT>
inline auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) noexcept -> bool;

/// @brief Count amount of substrings in string
///
/// @param str
/// @param substr
/// @return amount of occurances of @param substr in @param str
template<typename CharT>
inline auto SubstringCount(const std::basic_string<CharT> &str, const std::basic_string<CharT> &substr) noexcept -> std::size_t;

/// @brief Get char count of raw string array
///
/// @param first begin of c-style string array
/// @param last past the end element of c-style string array
/// @return charachter count of all strings in array
template<typename CharT>
inline constexpr auto RawStringArrayLength(const CharT **first, const CharT **last) noexcept -> std::size_t; 

/// @brief Connect raw string array into single string with delimetr
///
/// @param first begin of c-style string array
/// @param last past the end element of c-style string array
/// @param delim (default - "")
/// @return string with elements from @param first to @param last separated by @param delim 
template<typename CharT>
inline auto ConcatStringArray(const CharT **first, const CharT **last, const std::basic_string<CharT> &delim = "") noexcept -> std::basic_string<CharT>;

/// @brief strlen but templated for different char types
///
/// @param str
/// @return count of chars in @param str
template<typename CharT>
inline constexpr auto StrLen(const CharT *str) noexcept -> std::size_t;



template<typename CharT>
inline auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>
{
  const std::basic_string<CharT> &path = std::filesystem::current_path().generic_string<CharT>();
  if(!StringStartsWith(path, str))
    return str;
  return std::basic_string<CharT>(str).replace(0, path.size() + 1, ""); // + 1 for slash
}

template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim) -> std::vector<std::basic_string<CharT>>
{
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
  result.shrink_to_fit();

	return result;
}

template<typename CharT>
inline auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) noexcept -> bool
{
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
inline constexpr auto RawStringArrayLength(const CharT **first, const CharT **last) noexcept -> std::size_t
{
  std::size_t result = 0;
	for(; first != last; first++)
		result += StrLen(*first);

	return result;
}

template<typename CharT>
inline auto ConcatStringArray(const CharT **first, const CharT **last, const std::basic_string<CharT> &delim) noexcept -> std::basic_string<CharT>
{
	std::string result;
	result.reserve(RawStringArrayLength(first, last) + (last - first - 1) * delim.size());

  for(; first != last; first++)
  {
		result += *first;
		result += delim;
	}

	return result;
}

template <typename CharT>
inline constexpr auto StrLen(const CharT *str) noexcept -> std::size_t
{
  std::size_t result = 0;
  while(*str != static_cast<CharT>('\0'))
	{
    result++;
		str++;
	}

  return result;
}

#endif // STRING_UTILS_HPP