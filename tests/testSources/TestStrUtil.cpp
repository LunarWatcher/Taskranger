#include "catch2/catch.hpp"

#include "taskranger/util/StrUtil.hpp"

TEST_CASE("Splitting", "[StrSplit]") {
    const std::string raw = "this,is,a,string";
    std::vector<std::string> output1 = taskranger::StrUtil::splitString(raw, ",");
    std::vector<std::string> output2 = taskranger::StrUtil::splitString(raw, ",", 2);

    // Test limits
    REQUIRE(output1.size() == 4);
    REQUIRE(output2.size() == 3);

    // Verify splits
    REQUIRE(output1.at(0) == "this");
    REQUIRE(output1.at(3) == "string");
    REQUIRE(output2.at(2) == "a,string");
}

TEST_CASE("Conversion", "[StrConv]") {
    const std::vector<std::string> rawNums = {"-14", "15", "7721678321", "42", "997321"};
    const std::vector<unsigned long long> expected = {0ul, 15ul, 7721678321ul, 42ul, 997321ul};

    std::vector<unsigned long long> outputVector = {};
    taskranger::StrUtil::strVecToUll(rawNums, outputVector);
    REQUIRE(outputVector.size() == rawNums.size());
    for (size_t i = 0; i < outputVector.size(); i++) {
        REQUIRE(outputVector.at(i) == expected.at(i));
    }
}
