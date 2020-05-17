#ifndef TASKRANGER_UTIL_STRUTIL_HPP
#define TASKRANGER_UTIL_STRUTIL_HPP

#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace taskranger {
namespace Util {

    inline std::vector<std::string> splitString(std::string input, const std::string& delimiter, int limit = -1) {
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
    inline std::string toString(const nlohmann::json& json, const std::string& vectorPolicy = " ",
                                const std::string& nullPolicy = "null") {
        if (json.is_string()) {
            // Strings are just strings, but these can't be dumped.
            // Dumping them results in "annoying quotes around the output" because
            // it's meant for re-parsing
            return json.get<std::string>();
        } else if (json.is_null()) {
            return nullPolicy;
        } else if (json.is_boolean() || json.is_number()) {
            // If it's a number or a bool, it can be dumped
            // without artifacts. is_number is guaranteed to return
            // true for _all_ types of numbers; float or integer, signed or unsigned.
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
                if (i != json.size() - 1) raw += vectorPolicy;
            }
            return raw;
        }
        std::cout << "Sorry, I have no idea what happened here." << std::endl;
        std::cout << json.dump() << std::endl;
        throw std::runtime_error("An unexpected error occured");
    }

} // namespace Util
} // namespace taskranger

#endif
