#include <filesystem>
#define CATCH_CONFIG_RUNNER
#include "catch2/catch_session.hpp"

#include <iostream>

// This is the primary testing file. Specifically, this defines the entry point.
// It's also empty because Catch2 is super nice. All tests are defined in .cpp
// files, and it handles the rest of the clusterfuck setup. Really nice :D

int main(int argc, const char* argv[]) {

    const static auto directory = "./tests/raw/data";

    // Start test; make sure the data directory isn't polluted
    if (std::filesystem::exists(directory)) {
        std::cout << "Data directory dirty; removing previously produced data..." << std::endl;
        if (!std::filesystem::remove_all(directory)) {
            throw "ABORT! cannot delete ./tests/raw/data";
        }
    }

    // Run the tests
    return Catch::Session().run(argc, argv);
}
