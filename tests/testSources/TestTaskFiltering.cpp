#include "catch2/catch_test_macros.hpp"

#include "taskranger/data/Environment.hpp"
#include "taskranger/data/Task.hpp"
#include "taskranger/input/InputData.hpp"
#include "taskranger/util/TaskFilter.hpp"
#include "util/LoadConfig.hpp"
#include <algorithm>

using taskranger::InputData;
using taskranger::TaskFilter::Filter;
typedef std::shared_ptr<InputData> InputPtr;

TEST_CASE("TestFilterProject", "[TaskFilterProject]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();
    std::vector<std::string> keys = {"test", "@test"};

    for (auto& project : keys) {
        INFO(project);
        dataPtr->data["project"] = project;

        auto testLegacyProject = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());
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
    auto testTags = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());
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
    auto testNotTags = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());
    // Note: tag exclusion also includes tagless posts (because technically, they match the criteria).
    REQUIRE(testNotTags.size() == 3);

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
    auto testContains = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());

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
        auto testOutOfRange = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());

        // There's 4 tasks in the test data, and only 3 valid IDs passed.
        // This should return a JSON object containing 3 items, and it
        // should not throw.
        REQUIRE(testOutOfRange.size() == 3);
    }

    // Subtest 2: check invalid numbers
    SECTION("Test invalid numbers") {
        // Additionally, entirely invalid IDs should fail silently
        dataPtr->data["ids"] = "1,2,sqrt(3),3^4,LOOKATMEI'MSHOUTYTEXT:DDDD";
        auto testInvalid = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());
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
    auto testPrefixFilter = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());

    REQUIRE(testPrefixFilter.size() == 1);
    REQUIRE(testPrefixFilter.at(0)->getTaskJson().at("project") == "@bogus");
}

TEST_CASE("Date filtering", "[DateFiltering]") {
    const double earliestDate = 1591401600000.0;
    // const double middleDate = 1593993600000.0;
    const double latestDate = 1594044564000.0;

    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();
    dataPtr->data["age.after"] = "RAW" + std::to_string(earliestDate - 40000.0);

    auto testAfterOperator = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());
    REQUIRE(testAfterOperator.size() == 3);

    // The task filtering system isn't meant for sequential testing
    for (auto& task : json->getDatabase()) {
        task->reset();
    }

    dataPtr->data["age.before"] = "RAW" + std::to_string(latestDate);
    auto testComboOperator = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());

    REQUIRE(testComboOperator.size() == 2);

    // Top it off with an ID check
    dataPtr->data["ids.greater"] = "1";
    auto testIdDateCombo = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());
    REQUIRE(testIdDateCombo.size() == 1);
}

TEST_CASE("Date filtering with formats", "[DateFilteringFormats]") {
    const double latestDate = 1594044564000.0;

    INFO("Loading config and database...");
    LOAD_CONFIG("TaskFiltering.trconf");

    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);
    InputPtr dataPtr = std::make_shared<InputData>();

    // Should be fine; this invokes the current year, which should be greater than 2020
    // for all dates.
    dataPtr->data["age.day.before"] = "15.01";
    auto testBeforeOp = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());

    // Since all the ages were added last year, it should return all three tasks.
    // No idea if this is a good way to write tests or not.
    // Should be fine, I doubt anyone is gonna run the code code I just wrote a year ago :P
    // (but if you do, and changing the system time doesn't count, do let me know. Would be
    // interested in your means of time travel :eyes:)
    REQUIRE(testBeforeOp.size() == 3);

    dataPtr->data.erase("age.day.before");
    dataPtr->data["age.default.after"] = "1.7.2020 00:00:00";

    auto testAfterOp = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());

    // One is june, the other two are july. Two are still unmarked and therefore don't matter
    REQUIRE(testAfterOp.size() == 2);

    dataPtr->data.erase("age.default.after");
    dataPtr->data["age.zoned"] = "6.7.2020 03:09:24T+0";

    auto testFormatNoOp = Filter::createFilter(dataPtr).filterTasks(json->getDatabase());
    // +- 12 hours means there's two tasks included.
    // Same day would also mean that, even if there's no such check at this time.
    REQUIRE(testFormatNoOp.size() == 2);
}

TEST_CASE("Test automatic block", "[AUTOB]") {
    INFO("Loading config and database...");
    LOAD_CONFIG("TaskFiltering.trconf");
    auto json = taskranger::Environment::getInstance()->getDatabase("active.json", true);

    SECTION("Vtag not supplied as arg") {
        InputPtr dataPtr = std::make_shared<InputData>();
        INFO("Running filter");
        auto filtered =
                Filter::createFilter(dataPtr).disableConditionally("tags", "+BLOCKED").filterTasks(json->getDatabase());
        INFO("Requirements");
        REQUIRE(filtered.size() == 4);
        for (auto& task : filtered) {
            auto tJson = task->getTaskJson();
            REQUIRE(tJson.find("depends") == tJson.end());
        }
    }

    SECTION("VTag supplied as arg") {
        InputPtr dataPtr = std::make_shared<InputData>();
        INFO("Adding override...");
        dataPtr->data["tags"] = "+BLOCKED";
        INFO("Running filter");
        auto filtered =
                Filter::createFilter(dataPtr).disableConditionally("tags", "+BLOCKED").filterTasks(json->getDatabase());
        INFO("Requirements");
        REQUIRE(filtered.size() == 1);
        for (auto& task : filtered) {
            auto tJson = task->getTaskJson();
            REQUIRE(tJson.find("depends") != tJson.end());
        }
    }
}
