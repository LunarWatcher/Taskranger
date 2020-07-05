#include <catch2/catch.hpp>

#include "taskranger/util/DatetimeUtil.hpp"

TEST_CASE("Parse date", "[ParseDate]") {
    using namespace taskranger;

    const std::string baseDate = "02.07.2020 18:29:32T+0200";
    const std::string baseFormat = "dd.MM.y HH:mm:ss'T'z";

    REQUIRE(DateTimeUtil::parseTime(baseFormat, baseDate) == 1593707372000.0);
}

TEST_CASE("Format date", "[FormatDate]") {
    using namespace taskranger;
    const double baseDate = 1593707372000.0;
    // Cannot get more resolution; using dd doesn't work because some timezones are already on the 3rd with
    // this specific timestamp. That also excludes HH:mm:ss and z.
    // Just validating the month alone is a sign it works, however.
    auto baseFormat = "MM.y";
    REQUIRE(DateTimeUtil::formatDate(baseDate, baseFormat) == "07.2020");
}
