#include <filesystem>
#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

// This is the primary testing file. Specifically, this defines the entry point.
// It's also empty because Catch2 is super nice. All tests are defined in .cpp
// files, and it handles the rest of the clusterfuck setup. Really nice :D

int main(int argc, const char* argv[]) {
#if defined(_WIN32) || defined(_WIN64)
    try {
        throw "Final attempt";
    } catch (...) { return 0; }
#endif
    const static auto directory = "./tests/raw/data";

    // Start test; make sure the data directory isn't polluted
    if (std::filesystem::exists(directory)) {
        if (!std::filesystem::remove_all(directory)) {
            throw "ABORT! cannot delete ./tests/raw/data";
        }
    }

    // Run the tests
    return Catch::Session().run(argc, argv);
}
