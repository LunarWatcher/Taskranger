#ifndef TASKRANGER_UTIL_TERMUTILS_HPP
#define TASKRANGER_UTIL_TERMUTILS_HPP

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

#else

#endif

namespace taskranger {

namespace TermUtils {

inline int getWidth() {
    throw std::runtime_error("Not implemented yet");
}

}

}

#endif
