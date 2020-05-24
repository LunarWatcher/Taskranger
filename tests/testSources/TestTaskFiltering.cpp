#include <catch2/catch.hpp>

#include "taskranger/input/InputData.hpp"
#include "taskranger/util/TaskFilter.hpp"
#include <algorithm>

using taskranger::InputData;
// clang-format off
const nlohmann::json baseJson = {
    {
        {"description", "Test task for unit tests"},
        {"uuid", "3c05e648-64a9-4772-9ee3-36939e46d377"}
    }, {
        {"description", "Test task 2 for unit tests"},
        {"project", "@test"},
        {"uuid", "20a6af71-8627-4f79-a7b3-935b36c8ce23"}
    }, {
        {"description", "Test task 3 for unit tests"},
        {"project", "@test"},
        {"tags", std::vector<std::string>{"+tag"}},
        {"uuid", "477be9dc-a347-45d2-9a9c-93f04a7acd84"}
    }, {
        {"description", "Test task 4 for unit tests"},
        {"project", "@bogus"},
        {"tags", std::vector<std::string>{"+tag", "+tag2"}}
    }
};
// clang-format on

TEST_CASE("TestFilterProject", "[TaskFilterProject]") {
    auto mutableCopy = baseJson;

    std::shared_ptr<InputData> dataPtr = std::make_shared<InputData>();
    std::vector<std::string> keys = {"test", "@test"};

    for (auto& project : keys) {
        INFO(project);
        dataPtr->project = project;

        nlohmann::json testLegacyProject = taskranger::TaskFilter::filterTasks(mutableCopy, dataPtr, {});
        REQUIRE(testLegacyProject.size() == 2);
        for (auto& task : testLegacyProject) {
            REQUIRE(task.find("project") != task.end());
            REQUIRE(task.at("project") == "@test");
        }
    }
}

TEST_CASE("TestFilterTags", "[TaskFilterTags]") {
    auto mutableCopy = baseJson;
    std::shared_ptr<InputData> dataPtr = std::make_shared<InputData>();
    dataPtr->tags = { "+tag" };
    nlohmann::json testTags = taskranger::TaskFilter::filterTasks(mutableCopy, dataPtr, {});
    REQUIRE(testTags.size() == 2);
    for (auto& task : testTags) {
        REQUIRE(task.find("tags") != task.end());
        auto& tags = task.at("tags");
        REQUIRE(std::find(tags.begin(), tags.end(), "+tag") != tags.end());
    }
}
