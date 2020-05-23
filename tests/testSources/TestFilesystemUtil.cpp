#include "catch2/catch.hpp"
#include <cstdlib>
#include <string>
#include <regex>
#include "taskranger/util/FilesystemUtil.hpp"

/**
 * Tests whether or not path expansion works as expected.
 */
TEST_CASE("Testing path expansion", "[FilesystemUtilPath]") {
    std::string expansion = taskranger::FilesystemUtil::expandUserPath("~/.taskranger/");
    std::string matchRegex;
#if defined(_WIN32) || defined(_WIN64)
    matchRegex = "(\\w:)?/users/[\\w \\.-]+/.taskranger/";
#else
    matchRegex = "/(home|users)/\\w+/.taskranger/";
#endif
    const std::regex regexObj(matchRegex, std::regex::icase);
    std::smatch match;
    // This is only printed if the test fails
    INFO("System expanded to " + expansion);
    REQUIRE(std::regex_match(expansion, match, regexObj));
}

#ifdef _WIN32

TEST_CASE("Fail", "[fail]") {
    REQUIRE(false)
}

#endif

