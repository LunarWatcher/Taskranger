#pragma once

// Cross-platform dependencies
#include <string>

namespace taskranger {
namespace Env {

#if defined(_WIN32) || defined(_WIN64)
#define IS_WINDOZE
/**
 * Wrapper around _wdupenv_s for Windows.
 * Returns @{fail} if the environment variable doesn't exist
 */
std::string safeGetWindows(const char* name, const std::string& fail);
#else

/**
 * Wrapper around getenv() on Unix, and secure_getenv() on distros
 * supporting the GNU extension
 */
std::string getUnix(const char* name, const std::string& fail);

#endif

/**
 * Gets an environment variable. This attempts to make use of platform-specific
 * secure getenv variats (_dupenv_s on Windoze, secure_getenv on operating systems
 * with GNU extensions. See getenv(3)), and falls back to getenv on Unix/-like
 * systems without the GNU extension secure_getenv().
 *
 * The design here probably has to be revisited at some point
 */
std::string getEnv(const std::string& key, const std::string& fail = "");

} // namespace Env

} // namespace taskranger
