#include "catch2/catch_test_macros.hpp"

#include "taskranger/util/StrUtil.hpp"

using namespace taskranger::StrUtil;

TEST_CASE("Splitting", "[StrSplit]") {
    const std::string raw = "this,is,a,string";
    std::vector<std::string> output1 = splitString(raw, ",");
    std::vector<std::string> output2 = splitString(raw, ",", 2);

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
    strVecToUll(rawNums, outputVector);
    REQUIRE(outputVector.size() == rawNums.size());
    for (size_t i = 0; i < outputVector.size(); i++) {
        REQUIRE(outputVector.at(i) == expected.at(i));
    }
}

TEST_CASE("Reverse splitting", "[RevStrSplit]") {
    const std::string raw = "this,is,a,string";
    std::vector<std::string> output1 = reverseSplitString(raw, ",");
    std::vector<std::string> output2 = reverseSplitString(raw, ",", 2);

    // Test limits
    REQUIRE(output1.size() == 4);
    REQUIRE(output2.size() == 3);

    // Verify splits
    REQUIRE(output1.at(0) == "string");
    REQUIRE(output1.at(3) == "this");

    REQUIRE(output2.at(2) == "this,is");
    REQUIRE(output2.at(0) == "string");

    const std::string extDel = "Thisdelisdeladellongerdelstringdel!";
    std::vector<std::string> expected = {"!", "string", "longer", "a", "Thisdelis"};
    std::vector<std::string> output3 = reverseSplitString(extDel, "del", 4);

    REQUIRE(output3.size() == expected.size());
    for (size_t i = 0; i < output3.size(); i++) {
        REQUIRE(output3.at(i) == expected.at(i));
    }
}

TEST_CASE("Insensitive equals", "[InsensitiveEquals]") {
    REQUIRE(istrEquals("HeLlO WoRlD", "hello world"));
    REQUIRE(istrEquals("ShOVEL", "shOVEl"));
    REQUIRE(!istrEquals("SHOVEL!", "ShOvEl¡"));
}
