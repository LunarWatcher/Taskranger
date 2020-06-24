#pragma once

#include "date/date.h"
#include "date/tz.h"
#include "taskranger/data/Environment.hpp"
#include <chrono>
#include <cstdint>
#include <string>

namespace taskranger {

namespace DatetimeUtil {

inline int64_t parseRelative(int count, const std::string& substr) {
    auto now = std::chrono::system_clock::now();
    using namespace date;

    switch (substr[0]) {
    case 'h':
        now += std::chrono::hours(count);
        break;
    case 'd':
        now += days(count);
        break;
    case 'w':
        now += weeks(count);
        break;
    case 'm':
        now += months(count);
        break;
    case 'y':
        now += years(count);
        break;
    default:
        // Realistically, there's no reason what so ever for this to return 0
        // ... unless the date causes overflow or the user for whatever reason
        // wants to set up a meeting on January 1st 1970
        return 0;
    }
    return int64_t(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
}

int64_t parseTime(const std::string attribKey, const std::string& input) {

    auto config = Environment::getInstance()->getConfig();

    std::string format = "";

    if (attribKey.find(".") != std::string::npos) {
        auto split = StrUtil::splitString(attribKey, '.');
        auto secElem = split.at(1);
        if (secElem.size() != 0) {
            auto& dates = *config->findKey("dates");
            auto date = dates.find(secElem);
            if (date != dates.end()) {
                format = date.value().get<std::string>();
            } else {
                throw "Unknown date format: " + secElem + " -- does it exist?";
            }
        }
    }

    if (format.size() == 0) {
        // Try relative dates first
        // Relative dates always start with a number
        try {
            size_t pos;
            auto count = std::stoi(input, &pos);

            if (pos != input.size()) {
                auto attemptedTime = parseRelative(count, input.substr(pos));
                if (attemptedTime != 0) {
                    // if it isn't equal to 0, we've got a valid time, and
                    // we can finish up here.
                    // (See parseRelative for the rationale on 0)
                    return attemptedTime;
                }
            }
        } catch (std::invalid_argument&) {
            // If this is thrown, no conversion could be done, and the relative
            // date is invalid. Try to parse it normally instead
        }

        // if  the scheme is defaullt, and the format size is 0,
        // that means the default is implicit.
        format = config->findKey("dates")->at("default").get<std::string>();
    }

    /**
     * Here we go... Welcome to the problem section
     */
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> timepoint;

    /**
     * The parse function takes an istringstream input
     */
    std::istringstream stream(input);

    /**
     * And because the library is fuckign drunk, it parses the date relative to UTC instead of GMT.
     * There's only one undocumented workaround, and this is it:
     *
     * The offset, provided none is specified, has a side-effect on parsing.
     * If `offset = 1min`, it'll be shifted by one minute PROVIDED the date format
     * doesn't define a timezone.
     *
     * If the date format specifies a timezone, it takes priority over the manually supplied offset.
     * But since the API is drunk, the only way to get an offset is this overcomplicated piece of shit
     * that returns the offset **IN SECONDS**. (I mean, I can understand minutes and hours,
     * considering 04:30 is a valid offset, but fucking seconds??)
     *
     * Anyway, just using current_zone().get_info() doesn't work either. It appears to disregard DST,
     * and while DST is fucking garbage, it still exists
     *
     * So...
     */
    auto offset = // to get a valid offset
            std::chrono::duration_cast<std::chrono::minutes>( // we have to cast
                    date::make_zoned( // a zoned time
                            date::current_zone(), // using our current timezone
                            std::chrono::system_clock::now()) // with, for whatever fucking reason, the current time

                            .get_info() // from which we get the info, whatever that means
                            .offset); // which finally returns the offset
    // And then finally we can parse the date, which now parses with the current timezone if none
    // is supplied
    stream >> date::parse(format, timepoint, offset);

    // (meta) check whether it succeeded
    if (!stream) {
        throw "Failed to parse date (" + input + ") using \"" + format + "\"";
    }

    return int64_t(timepoint.time_since_epoch().count());
}

} // namespace DatetimeUtil

} // namespace taskranger
