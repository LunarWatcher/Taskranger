#pragma once

#include "taskranger/data/Environment.hpp"
#include "taskranger/util/StrUtil.hpp"

#include "unicode/bytestream.h"
#include "unicode/datefmt.h"
#include "unicode/gregocal.h"
#include "unicode/reldatefmt.h"
#include "unicode/smpdtfmt.h"
#include "unicode/unistr.h"

#include <chrono>
#include <cmath>
#include <limits>
#include <regex>
#include <stdexcept>
#include <string>

namespace taskranger {
namespace DateTimeUtil {

using std::chrono::duration;
using millis = duration<double, std::milli>;
using seconds = duration<double>;
using minutes = duration<double, std::ratio<60>>;
using hours = duration<double, std::ratio<3600>>;
using days = duration<double, std::ratio<86400>>;
using weeks = duration<double, std::ratio<604800>>;
using months = duration<double, std::ratio<2629746>>;
using years = duration<double, std::ratio<31556952>>;

inline double currTime() {
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
            .time_since_epoch()
            .count();
}

inline double parseRelative(const std::string& inputDate, double relativeTo = -1) {
    double parseRelativeTo =
            relativeTo >= 0
                    ? relativeTo
                    : std::chrono::duration_cast<millis>(std::chrono::system_clock::now().time_since_epoch()).count();

    try {
        size_t parseEndPos;
        int numUnits = std::stoi(inputDate, &parseEndPos);
        if (parseEndPos == inputDate.size()) {
            return -1;
        }

        // Potentially a microoptimization, but only go through the if statement if the
        // inputDate's length is greater than or equal to three. This
        // prevents wasted time trying to parse too short dates.
        if (parseEndPos == 0 && inputDate.size() >= 3) {}

        switch (inputDate[parseEndPos]) {
        case 'd':
            return std::chrono::duration_cast<millis>(days(numUnits)).count() + parseRelativeTo;
        case 'm':
            return std::chrono::duration_cast<millis>(months(numUnits)).count() + parseRelativeTo;
        case 'y':
            return std::chrono::duration_cast<millis>(years(numUnits)).count() + parseRelativeTo;
        case 'w':
            return std::chrono::duration_cast<millis>(weeks(numUnits)).count() + parseRelativeTo;
        case 'h':
            return std::chrono::duration_cast<millis>(hours(numUnits)).count() + parseRelativeTo;
        default:
            return -1;
        }

    } catch (std::invalid_argument&) {
        // ign0re
    }

    using namespace icu;
    using namespace std::literals;
    using CF = UCalendarDateFields;

    // This is blatantly ignored, because I cannot be reasonably expected to check this for _every single call_
    // What a backwater API though.
    UErrorCode status = U_ZERO_ERROR;

    auto cal = Calendar::createInstance(status);
    cal->setTime(UDate{parseRelativeTo}, status);

    if (inputDate == "today" || inputDate == "eod") {
        cal->set(CF::UCAL_HOUR_OF_DAY, 23);
        cal->set(CF::UCAL_MINUTE, 59);
        cal->set(CF::UCAL_SECOND, 0); // Could strictly speaking be 59, but because floating point is the fucking worst,
                                      // a 60 second margin prevents any nasty fuckups
    } else if (inputDate == "tomorrow") {
        cal->add(CF::UCAL_DAY_OF_MONTH, 1, status);
        cal->set(CF::UCAL_HOUR_OF_DAY, 0);
        cal->set(CF::UCAL_MINUTE, 0);
        cal->set(CF::UCAL_SECOND, 0);
    } else {
        return -1;
    }
    return cal->getTime(status);
}

inline double parseTime(const std::string& format, const std::string& inputDate) {
    auto relative = parseRelative(inputDate);
    if (relative != -1) {
        return relative;
    }

    using namespace std::literals;
    using namespace icu;
    using CF = UCalendarDateFields;
    // TODO: try to do something slightly more with this
    // than pretending it's useless
    UErrorCode status = U_ZERO_ERROR;

    SimpleDateFormat sdf(UnicodeString(format.c_str()), status);
    // Get the calendar early; before anything is parsed, it is set to the current
    // day and time, which we can use later C:
    Calendar* cal = const_cast<Calendar*>(sdf.getCalendar());

    // The _current_ day, month, and year.
    // Used if there's missing components and TR has to make assumptions wrt.
    // dates.
    auto day = cal->get(CF::UCAL_DAY_OF_MONTH, status);
    auto month = cal->get(CF::UCAL_MONTH, status);
    auto year = cal->get(CF::UCAL_YEAR, status);

    ParsePosition pp;
    UDate time = sdf.parse(inputDate.c_str(), pp);
    if (pp.getErrorIndex() != -1) {
        throw "Failed to parse:\n" + inputDate + "\n" + StrUtil::padString("^", pp.getErrorIndex() + 1) + "\nWith\n" +
                format;
    }

    cal->setTime(time, status);

    // hack for making sure the day, month, and year are all set
    // Base check; search for letters implying all of the three
    std::smatch unused;
    if (!std::regex_search(format, unused, std::regex("[D]"))) {
        // D: day of year (implies month and year)

        if (!std::regex_search(format, unused, std::regex("[dFg]"))) {
            cal->set(CF::UCAL_DAY_OF_MONTH, day);
        }

        if (!std::regex_search(format, unused, std::regex("[ML]"))) {
            // No month specified
            cal->set(CF::UCAL_MONTH, month);
        }

        if (!std::regex_search(format, unused, std::regex("[YyuU]"))) {
            cal->set(CF::UCAL_YEAR, year);
        }
    }

    // Returns real UNIX time (relative to GMT, not relative to localtime. Phew)
    // (Milliseconds btw. Not like I can't found digits or anything :kekw:)
    time = cal->getTime(status);
    return time;
}

inline double parseTimeWith(const std::string& formatName, const std::string& inputDate) {
    auto dateJson = *Environment::getInstance()->getConfig()->findKey("dates");
    auto it = dateJson.find(formatName);
    if (it == dateJson.end()) {
        throw "Format " + formatName + " not found.";
    }
    return parseTime(it->get<std::string>(), inputDate);
}

inline double parseTimeKey(const std::string& key, const std::string& inputDate) {
    auto split = StrUtil::splitString(key, '.');
    std::string format;
    auto dateJson = *Environment::getInstance()->getConfig()->findKey("dates");
    if (split.size() == 1) {
        format = "default";
    } else {
        format = split.at(1);
    }
    return parseTimeWith(format, inputDate);
}

inline std::string formatDate(const double& timestamp, const std::string& iFormat = "") {
    using namespace icu;
    auto format = iFormat.size() == 0
                          ? Environment::getInstance()->getConfig()->findKey("dates")->at("default").get<std::string>()
                          : iFormat;

    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat sdf(UnicodeString(format.c_str()), status);

    UnicodeString buff;
    std::string output;
    sdf.format(timestamp, buff, status);
    buff.toUTF8String(output);

    return output;
}

inline std::string formatRelative(const double& timestamp) {
    using namespace icu;

    auto now = std::chrono::duration_cast<millis>(std::chrono::system_clock::now().time_since_epoch());
    auto relativeUnix = now - millis(timestamp);

    auto direction =
            relativeUnix.count() < 0 ? UDateDirection::UDAT_DIRECTION_NEXT : UDateDirection::UDAT_DIRECTION_LAST;
    UDateRelativeUnit unit = UDateRelativeUnit::UDAT_RELATIVE_YEARS;
    auto absDate = direction == UDateDirection::UDAT_DIRECTION_NEXT ? -relativeUnix : relativeUnix;

    double count;

    if (absDate < minutes(1)) {
        unit = UDateRelativeUnit::UDAT_RELATIVE_SECONDS;
        count = seconds(absDate).count();
    } else if (absDate < hours(1)) {
        unit = UDateRelativeUnit::UDAT_RELATIVE_MINUTES;
        count = minutes(absDate).count();
    } else if (absDate < days(1)) {
        unit = UDateRelativeUnit::UDAT_RELATIVE_HOURS;
        count = hours(absDate).count();
    } else if (absDate < months(1)) {
        unit = UDateRelativeUnit::UDAT_RELATIVE_DAYS;
        count = days(absDate).count();
    } else if (absDate < years(1)) {
        unit = UDateRelativeUnit::UDAT_RELATIVE_MONTHS;
        count = months(absDate).count();
    } else {
        count = years(absDate).count();
    }
    count = std::round(count);
    UnicodeString buff;
    std::string output;
    UErrorCode status = U_ZERO_ERROR;
    RelativeDateTimeFormatter formatter(status);

    formatter.format(count, direction, unit, buff, status);
    buff.toUTF8String(output);
    return output;
}

} // namespace DateTimeUtil
} // namespace taskranger
