#ifndef TASKRANGER_UTIL_STRUTIL_HPP
#define TASKRANGER_UTIL_STRUTIL_HPP

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace taskranger {
namespace StrUtil {

std::vector<std::string> splitString(std::string input, const char delimiter, int limit = -1);

/**
 * Splits a string by a provided delimiter. If the length of the delimiter is 1,
 * splitString(std::string, const char) will be used instead, provided the limit
 * is -1.
 */
std::vector<std::string> splitString(std::string input, const std::string& delimiter, int limit = -1);

/**
 * Equivalent to splitString, but it uses rfind (reverse find) instead of find.
 */
std::vector<std::string> reverseSplitString(std::string input, const std::string& delimiter, int limit = -1);

/**
 * Converts JSON to a string.
 *
 * @param json          The JSON to convert
 * @param vectorPolicy  What character to use when printing vector values
 */
std::string toString(
        const nlohmann::json& json, const std::string& vectorPolicy = " ", const std::string& nullPolicy = "null");

/**
 * Converts a vector of strings to a vector of unsigned long longs
 *
 * For invalid numbers, the number is converted to a 0 instead.
 * This is mainly intended to be used with IDs, so any return values
 * that are 0 can be considered invalid.
 *
 * This should not be used with conversions where 0 is a valid ID.
 */
void strVecToUll(const std::vector<std::string>& strIds, std::vector<unsigned long long>& out);

/**
 * Compares two strings. Not case-sensitive
 */
bool istrEquals(const std::string& a, const std::string& b);

/**
 * Converts a string to lower-case
 */
std::string toLower(std::string input);

/**
 * Checks if a string starts with a given prefix
 */
bool startsWith(const std::string& in, const std::string& prefix);

std::string padString(const std::string& padTarget, size_t padSize);

inline int64_t stoi64(const std::string& raw, size_t* pos = nullptr) {
    if constexpr (std::is_same_v<int64_t, signed long long>) {
        return std::stoll(raw, pos);
    }
    return std::stol(raw, pos);
}

} // namespace StrUtil
} // namespace taskranger

#endif
