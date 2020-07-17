#include "catch2/catch.hpp"

#include "taskranger/commands/ModifyCommand.hpp"
#include "taskranger/data/Environment.hpp"
#include "util/CaptureStreams.hpp"
#include "util/LoadConfig.hpp"
#include <iostream>
#include <string>

/**
 * This exists to make sure the IO redirection works
 */
TEST_CASE("Test IO modification", "[IOStreamMod]") {
    CaptureStreams::StreamCapturer hijackOutput(std::cout);
    CaptureStreams::StreamCapturer hijackInput(std::cin);

    hijackInput.getBuffer() << "Hello World!\n";
    std::string line = "Goodbye";
    getline(std::cin, line);
    std::cout << line;
    hijackInput.restore();
    hijackOutput.restore();

    // Test output to make sure restore() works
    std::cout << "This output should not be captured\n";
    if (hijackOutput.getBufferContent() != "Hello World!") {
        INFO("Expected \"Hello World!\", got \"" + hijackOutput.getBufferContent() + "\"");
        // This is a horrible practice, but if the redirect
        // testing fails, we HAVE to abort the tests. This
        // is critical for the CI to make sure it doesn't hang
        // indefinitely, waiting for user input.
        //
        // There's no good things that can happen by letting
        // this slide
        abort();
    }
}
/*
TEST_CASE("Add and remove tags", "[ModifyTags]") {
    LOAD_CONFIG("TaskFiltering.trconf");
    using namespace taskranger;
    using CaptureStreams::StreamCapturer;

    ModifyCommand command;
    INFO("Hijacking input and output streams");
    StreamCapturer hijackOutput(std::cout);
    StreamCapturer hijackInput(std::cin);
    INFO("Disabling database writes...");
    auto active = Environment::getInstance()->getDatabase("active.json", true);
    auto unused = Environment::getInstance()->getDatabase("completed.json", false);
    unused->demoMode = active->demoMode = true;

    INFO("Preparing input data");
    auto inputData = Environment::getInstance()->getInputData();
    auto& data = inputData->data;
    // 1 + 4 tests tag addition and removal when:
    // * The task has a tag that's meant to be added
    // * The task does not have a tag that's meant to be added
    // * The task has a tag that's meant to be removed
    // * The task does not have a tag that's meant to be removed
    data["ids"] = "1,4";
    inputData->tags = {"+tag", "-tag2"};

    hijackInput.getBuffer() << "y\n";
    INFO("Executing command");
    command.run();
    INFO("Restoring IO control");
    hijackInput.restore();
    hijackOutput.restore();

    INFO("Checking stdout's content");
    std::string commandOutput = hijackOutput.getBufferContent();
    INFO(commandOutput);
    REQUIRE(commandOutput.find("2 tasks modified") != std::string::npos);

    auto activeDB = active->getDatabase();
    auto firstJson = activeDB.at(0)->getTaskJson();
    auto secondJson = activeDB.at(3)->getTaskJson();
    INFO("First JSON: " + firstJson.dump());
    REQUIRE(firstJson.at("tags").size() == 1);
    INFO("Second JSON: " + secondJson.dump());
    REQUIRE(secondJson.at("tags").size() == 1);

    REQUIRE(firstJson.at("tags").at(0) == "+tag");
    REQUIRE(secondJson.at("tags").at(0) == "+tag");
}*/
