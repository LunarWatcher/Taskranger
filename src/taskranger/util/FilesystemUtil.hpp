#ifndef TASKRANGER_UTIL_FILESYSTEMUTIL_HPP
#define TASKRANGER_UTIL_FILESYSTEMUTIL_HPP

#include <optional>
#include <regex>
#include <string>

#include <iostream>
#include "taskranger/util/ColorPrinter.hpp"

// Windows is the easiest one to detect.
// Basically, this first clause applies for everything that isn't Windows,
// which should be able to use these.
#if !defined(_WIN32) && !defined(_WIN64)
#include <pwd.h>
// Used for getuid
#include <unistd.h>
#include <sys/types.h>
#else
#include <windows.h>

namespace windoze {

inline const char* windowsGetEnv(const char* name) {
    const DWORD buffSize = 65535;
    std::wstring buff;
    buff.resize(buffSize);

    bufferSize = GetEnvironmentVariableA(name, buffer, buffSize);
    if(!bufferSize)
        return nullptr;
    buff.resize(bufferSize);
    return buff;
}

}
#endif

// required for getenv. Allegedly cross-platform
#include <cstdlib>

namespace taskranger {

namespace FilesystemUtil {

    /**
     * Joins paths.
     */
    std::string joinPath(const std::string& a, const std::string& b);

    /**
     * Detect separators.
     * This is a function to enable \\ and /
     */
    bool isSeparator(const char x);

    /**
     * Expands a user path (AKA a path starting with ~), independently of the OS.
     * This requires several OS-specific calls (specifically between Windows and UNIX, from what I can tell.
     * There's not much of a difference in this area between for an instance Linux and Mac. Special
     * implementation requirements will be taken when we get to a point where it's needed)
     */
    std::string expandUserPath(const std::string& inputPath);

} // namespace FilesystemUtil

} // namespace taskranger

#endif
