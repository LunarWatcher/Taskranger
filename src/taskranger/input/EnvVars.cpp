#include "EnvVars.hpp"
#include <string>
#if defined(HAS_SECURE_GETENV) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <stdlib.h>

namespace taskranger {

#ifdef IS_WINDOZE

std::string Env::safeGet(const char* name, const std::string& fail) {
    char* value = nullptr;
    size_t len = 0;
    errno_t err = _dupenv_s(&value, &len, name);
    if (err != 0 || value == nullptr)
        return fail;

    std::string retVal(value);
    return retVal;
}

#else

std::string Env::getUnix(const char* name, const std::string& fail) {
#ifdef HAS_SECURE_GETENV
    char* output = secure_getenv(name);
    if (!output)
        return fail;
    return std::string(output);
#else
#pragma message("WARNING: your OS doesn't have secure_getenv(). If this is a mistake, " \
                "please open an issue on GitHub.")
    char* output = getenv(name);
    if (!output)
        return fail;
    return std::string(output);
#endif
}

#endif

std::string Env::getEnv(const std::string& key, const std::string& fail) {
#ifdef IS_WINDOZE
    return safeGet(key.c_str(), fail);
#else
    return getUnix(key.c_str(), fail);
#endif
}

} // namespace taskranger
