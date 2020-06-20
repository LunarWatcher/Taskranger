#include "catch2/catch.hpp"
#include "taskranger/commands/AddCommand.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/input/EnvVars.hpp"
#include "util/LoadConfig.hpp"
#include <filesystem>

TEST_CASE("Verify AddCommand output", "[AddCommandOutput]") {
    LOAD_CONFIG("AddTest.trconf");
    REQUIRE(config->getString("dataDir") == "./tests/raw/data/AddTest");

    auto environment = taskranger::Environment::getInstance();
    auto addCommand = environment->getCommands()->getCommand("add");
    auto inputData = environment->getInputData();
    // clang-format off
    inputData->data = {
        {"description", "Make sure AddCommand works"},
        {"project", "@yayeet"}
    };
    // clang-format on
    inputData->tags = {"+test"};

    addCommand->run();

    REQUIRE(std::filesystem::exists("./tests/raw/data"));
    REQUIRE(std::filesystem::exists("./tests/raw/data/AddTest"));
    REQUIRE(std::filesystem::exists("./tests/raw/data/AddTest/active.json"));

    auto database = environment->getDatabase("active.json", true);
    auto& raw = *database->getRawDatabase();
    REQUIRE(raw.size() == 1);
    REQUIRE(raw.at(0).at("description") == "Make sure AddCommand works");
    REQUIRE(raw.at(0).at("tags").size() == 1);
    REQUIRE(raw.at(0).at("project") == "@yayeet");
}

TEST_CASE("Test UDA", "[TestUDA]") {
    LOAD_CONFIG("UDATest.trconf");
    REQUIRE(config->getString("dataDir") == "./tests/raw/data/UDATest/");

    auto environment = taskranger::Environment::getInstance();
    auto addCommand = environment->getCommands()->getCommand("add");
    auto inputData = environment->getInputData();
    auto database = environment->getDatabase("active.json", true);
    database->demoMode = true;

    // Test string
    // clang-format off
    inputData->data = {
        {"description", "Yes"},
        {"string", "abcd"}
    };
    // clang-format on
    addCommand->run();
    REQUIRE(database->size() == 1);
    REQUIRE(database->getRawDatabase()->at(0).at("string") == "abcd");

    // Test number
    // clang-format off
    inputData->data = {
        {"description", "yes"},
        {"number", "1234"}
    };
    // clang-format on
    addCommand->run();
    REQUIRE(database->size() == 2);
    REQUIRE(database->getRawDatabase()->at(1).at("number") == 1234);

    // Test invalid number
    // clang-format off
    inputData->data = {
        {"description", "Yes"},
        {"number", "https://www.youtube.com/watch?v=lXMskKTw3Bc"}
    };
    // clang-format on

    REQUIRE_THROWS_AS(addCommand->run(), std::string);

    // Test invalid value
    // clang-format off
    inputData->data = {
        {"description", "yes"},
        {"stringRestrict", "I'm not a valid value for this field, so my call to AddCommand will throw"}
    };
    // clang-format on
    REQUIRE_THROWS_AS(addCommand->run(), std::string);
}
