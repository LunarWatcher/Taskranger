#include <catch2/catch.hpp>

#include "taskranger/data/Environment.hpp"
#include "taskranger/data/Task.hpp"
#include "taskranger/input/InputData.hpp"
#include "taskranger/util/TaskFilter.hpp"
#include "util/LoadConfig.hpp"
#include <algorithm>

using taskranger::InputData;
typedef std::shared_ptr<InputData> InputPtr;

TEST_CASE("TestFilterProject", "[TaskFilterProject]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();
    std::vector<std::string> keys = {"test", "@test"};

    for (auto& project : keys) {
        INFO(project);
        dataPtr->data["project"] = project;

        auto testLegacyProject = taskranger::TaskFilter::filterTasks(json->getDatabase(), dataPtr);
        REQUIRE(testLegacyProject.size() == 2);
        for (auto& taskObj : testLegacyProject) {
            auto& task = taskObj->getTaskJson();
            REQUIRE(task.find("project") != task.end());
            REQUIRE(task.at("project") == "@test");
        }
    }
}

TEST_CASE("TestFilterTags", "[TaskFilterTags]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();
    dataPtr->tags = {"+tag"};
    auto testTags = taskranger::TaskFilter::filterTasks(json->getDatabase(), dataPtr);
    REQUIRE(testTags.size() == 2);
    for (auto& taskObj : testTags) {
        auto& task = taskObj->getTaskJson();
        REQUIRE(task.find("tags") != task.end());
        auto& tags = task.at("tags");
        REQUIRE(std::find(tags.begin(), tags.end(), "+tag") != tags.end());
    }
}

TEST_CASE("Filter operator: not", "[TaskFilterOpNot]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();

    // Test if tag exclusion works
    dataPtr->data["tags.not"] = "+tag";
    auto testNotTags = taskranger::TaskFilter::filterTasks(json->getDatabase(), dataPtr);
    // Note: tag exclusion also includes tagless posts (because technically, they match the criteria).
    REQUIRE(testNotTags.size() == 2);

    // Make sure we excluded the right shit
    // If this worked, +tag shouldn't be present anywhere.
    for (auto& taskObj : testNotTags) {
        auto& task = taskObj->getTaskJson();
        if (task.find("tags") != task.end()) {
            auto& tags = task.at("tags");
            REQUIRE(std::find(tags.begin(), tags.end(), "+tag") == tags.end());
        }
    }
}

TEST_CASE("Filter operator: contains", "[TaskFilterOpContains]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();

    dataPtr->data["description.contains"] = "Test task 4";
    auto testContains = taskranger::TaskFilter::filterTasks(json->getDatabase(), dataPtr);
    // Only one of the test tasks match this criteria
    REQUIRE(testContains.size() == 1);
}

TEST_CASE("Invalid and out of range IDs", "[TaskFilterIDs]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();
    // Subtest 1: check out of range
    SECTION("Test out of range") {
        dataPtr->data["ids"] = "1,2,3,67483,42,31415";
        auto testOutOfRange = taskranger::TaskFilter::filterTasks(json->getDatabase(), dataPtr);
        // There's 4 tasks in the test data, and only 3 valid IDs passed.
        // This should return a JSON object containing 3 items, and it
        // should not throw.
        REQUIRE(testOutOfRange.size() == 3);
    }

    // Subtest 2: check invalid numbers
    SECTION("Test invalid numbers") {
        // Additionally, entirely invalid IDs should fail silently
        dataPtr->data["ids"] = "1,2,sqrt(3),3^4,LOOKATMEI'MSHOUTYTEXT:DDDD";
        auto testInvalid = taskranger::TaskFilter::filterTasks(json->getDatabase(), dataPtr);
        // 1 and 2 are the only valid IDs.
        // Not throwing is the main test
        REQUIRE(testInvalid.size() == 2);
    }
}

TEST_CASE("UUID filtering", "[UUIDs]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();
    dataPtr->data["ids"] = "abcd";
    auto testPrefixFilter = taskranger::TaskFilter::filterTasks(json->getDatabase(), dataPtr);
    REQUIRE(testPrefixFilter.size() == 1);
    REQUIRE(testPrefixFilter.at(0)->getTaskJson().at("project") == "@bogus");
}
