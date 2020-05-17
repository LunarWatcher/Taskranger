#ifndef TASKRANGER_UTIL_TERMUTILS_HPP
#define TASKRANGER_UTIL_TERMUTILS_HPP

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace taskranger {

namespace TermUtils {

inline int getWidth() {
#if defined(_WIN32) || defined(_WIN64)
    // Windows likes to yell. {{{
    CONSOLE_SCREEN_BUFFER_INFO csbi; 
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    // }}}
#else
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
#endif
}

}

}

#endif
