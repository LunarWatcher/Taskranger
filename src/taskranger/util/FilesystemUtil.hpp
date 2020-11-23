#ifndef TASKRANGER_UTIL_FILESYSTEMUTIL_HPP
#define TASKRANGER_UTIL_FILESYSTEMUTIL_HPP

#include <optional>
#include <regex>
#include <string>

#include "taskranger/util/ColorPrinter.hpp"
#include <iostream>

// Windows is the easiest one to detect.
// Basically, this first clause applies for everything that isn't Windows,
// which should be able to use these.
#if !defined(_WIN32) && !defined(_WIN64)
#include <pwd.h>
// Used for getuid
#include <sys/types.h>
#include <unistd.h>
#endif

// required for getenv. Allegedly cross-platform
#include "taskranger/util/CompatUtil.hpp"
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
