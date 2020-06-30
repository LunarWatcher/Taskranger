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

inline double parseRelative(const std::string& inputDate) {

    try {
        size_t parseEndPos;
        int numUnits = std::stoi(inputDate, &parseEndPos);
        if (parseEndPos == inputDate.size()) {
            return -1;
        }
        auto now = std::chrono::system_clock::now();
        switch (inputDate[parseEndPos]) {
        case 'd':
            return std::chrono::duration_cast<millis>((now + days(numUnits)).time_since_epoch()).count();
        case 'm':
            return std::chrono::duration_cast<millis>((now + months(numUnits)).time_since_epoch()).count();
        case 'y':
            return std::chrono::duration_cast<millis>((now + years(numUnits)).time_since_epoch()).count();
        case 'w':
            return std::chrono::duration_cast<millis>((now + weeks(numUnits)).time_since_epoch()).count();
        case 'h':
            return std::chrono::duration_cast<millis>((now + hours(numUnits)).time_since_epoch()).count();
        default:
            return -1;
        }

    } catch (std::invalid_argument&) { return -1; }
}

inline double parseTime(const std::string& format, const std::string& inputDate) {
    auto relative = parseRelative(inputDate);
    if (relative != -1) {
        return relative;
    }

    using namespace std::literals;
    using namespace icu_67;
    using CF = UCalendarDateFields;
    // TODO: try to do something slightly more with this
    // than pretending it's useless
    UErrorCode status = U_ZERO_ERROR;

    SimpleDateFormat sdf(UnicodeString(format.c_str()), status);
    // Get the calendar early; before anything is parsed, it is set to the current
    // day and time, which we can use later C:
    Calendar* cal = const_cast<Calendar*>(sdf.getCalendar());
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

    time = cal->getTime(status);

    std::cout << std::fixed << time << std::endl;
    UnicodeString s;
    sdf.format(time, s, status);
    std::string fmt;
    s.toUTF8String(fmt);
    std::cout << fmt << std::endl << std::endl;
    return time;
}

inline std::string formatDate(const double& timestamp) {
    using namespace icu;
    auto format = Environment::getInstance()->getConfig()->findKey("dates")->at("default");

    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat sdf(UnicodeString(format.get<std::string>().c_str()), status);

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
        unit = UDateRelativeUnit::UDAT_RELATIVE_WEEKS;
        count = days(absDate).count();
    } else if (absDate < years(1)) {
        unit = UDateRelativeUnit::UDAT_RELATIVE_MONTHS;
        count = months(absDate).count();
    } else {
        count = years(absDate).count();
    }
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
