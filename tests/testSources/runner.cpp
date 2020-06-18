#include <filesystem>
#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"
#if defined(_WIN32) || defined(_WIN64)
#define THIS_IS_WINDOZE
#include <rtcapi.h>
#include <windows.h>

int exceptionHandler(LPEXCEPTION_POINTERS p) {
    printf("Fail\n");
    exit(1);
}
int runtimeCheckHandler(
        int errorType, const char* filename, int linenumber, const char* moduleName, const char* format, ...) {
    printf("Error: type %d at %s line %d in %s", errorType, filename, linenumber, moduleName);
    exit(1);
}
#endif
// This is the primary testing file. Specifically, this defines the entry point.
// It's also empty because Catch2 is super nice. All tests are defined in .cpp
// files, and it handles the rest of the clusterfuck setup. Really nice :D

int main(int argc, const char* argv[]) {
#ifdef THIS_IS_WINDOZE
    DWORD dwMode = SetErrorMode(SEM_NOGPFAULTERRORBOX);
    SetErrorMode(dwMode | SEM_NOGPFAULTERRORBOX);
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)&exceptionHandler);
    _RTC_SetErrorFunc(&runtimeCheckHandler);

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
