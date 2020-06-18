#pragma once

#if defined(_WIN32) || defined(_WIN64)
// Fuck you Windoze
#define THIS_IS_WINDOZE

#endif
#include <stdlib.h>
#include <string>

namespace TestUtil {

inline void localSetenv(const std::string& key, const std::string& value) {
#ifdef THIS_IS_WINDOZE
    errno_t err = _putenv_s(key.c_str(), value.c_str());
    if (err != 0) {
        throw "I can't be bothered with this: " + std::to_string(err);
    }
#else

    int err = setenv(key.c_str(), value.c_str(), true);
    if (err) {
        throw err;
    }
#endif
}

} // namespace TestUtil
