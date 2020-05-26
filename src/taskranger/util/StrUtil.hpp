#ifndef TASKRANGER_UTIL_STRUTIL_HPP
#define TASKRANGER_UTIL_STRUTIL_HPP

#include <cstdio>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace taskranger {
namespace StrUtil {

inline std::vector<std::string> splitString(std::string input, const char delimiter) {
    if (delimiter == 0)
        return {input};

    std::vector<std::string> out;
    std::stringstream stream(input);
    std::string line;
    while (getline(stream, line, delimiter)) {
        out.push_back(line);
    }
    return out;
}
inline std::vector<std::string> splitString(std::string input, const std::string& delimiter, int limit = -1) {
    // Special case: no delimiter.
    if (delimiter == "")
        return {input};

    // Optimize: use a stringstream instead of memory
    // altering operations when the delimiter is a single
    // char, and there is no limit
    if (limit == -1 && delimiter.size() == 1)
        return splitString(input, delimiter.at(0));

    std::vector<std::string> out;
    size_t pos = 0;
    std::string token;
    int count = 0;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        out.push_back(token);
        input.erase(0, pos + delimiter.length());
        count++;
        if (count == limit) {
            break;
        }
    }
    out.push_back(input);
    return out;
}

/**
 * Converts JSON to a string.
 *
 * @param json          The JSON to convert
 * @param vectorPolicy  What character to use when printing vector values
 */
inline std::string toString(
        const nlohmann::json& json, const std::string& vectorPolicy = " ", const std::string& nullPolicy = "null") {
    if (json.is_string()) {
        // Strings are just strings, but these can't be dumped.
        // Dumping them results in "annoying quotes around the output"
        // because it's meant for re-parsing
        return json.get<std::string>();
    } else if (json.is_null()) {
        return nullPolicy;
    } else if (json.is_boolean() || json.is_number()) {
        // If it's a number or a bool, it can be dumped
        // without artifacts. is_number is guaranteed to return
        // true for _all_ types of numbers; float or integer, signed or
        // unsigned.
        return json.dump();
    } else if (json.is_object()) {
        // Objects aren't meant to be printed in a nice way.
        // Just dump it and get it over with
        return json.dump();
    } else if (json.is_array()) {
        std::string raw;
        for (size_t i = 0; i < json.size(); i++) {
            auto& item = json.at(i);
            std::string itemValue = toString(item);
            raw += item;
            if (i != json.size() - 1)
                raw += vectorPolicy;
        }
        return raw;
    }
    std::cout << "Sorry, I have no idea what happened here.\n";
    std::cout << json.dump() << "\n";
    throw std::runtime_error("An unexpected error occured");
}

/**
 * Converts a vector of strings to a vector of unsigned long longs
 *
 * For invalid numbers, the number is converted to a 0 instead.
 * This is mainly intended to be used with IDs, so any return values
 * that are 0 can be considered invalid.
 *
 * This should not be used with conversions where 0 is a valid ID.
 */
inline void strVecToUll(const std::vector<std::string>& strIds, std::vector<unsigned long long>& out) {
    std::transform(
            strIds.begin(), strIds.end(), std::back_inserter(out), [](const std::string& id) -> unsigned long long {
                try {
                    // This saves some cycles. Drop empty strings
                    // and return 0 for negative numbers.
                    // stoull on negative numbers has weird behavior.
                    if (id.length() == 0 || id.at(0) == '-')
                        return 0;
                    return std::stoull(id);
                } catch (std::invalid_argument&) {
                    // Silently fail dead IDs.
                    // This might not be the best idea.
                    return 0;
                }
            });
}

} // namespace StrUtil
} // namespace taskranger

#endif
