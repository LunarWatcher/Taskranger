#ifndef TASKRANGER_UTIL_TERMUTILS_HPP
#define TASKRANGER_UTIL_TERMUTILS_HPP

#include <iostream>
#include <ostream>

#if defined(_WIN32) || defined(_WIN64)
#define WINDOZE
#include <io.h>
#include <windows.h>

// TTY check
#define IsATTY _isatty
#define FILENO _fileno

// ANSI support check
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
// Hard-code the variable.
// We can use this to tell if ANSI is supported or not
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#else
#include <sys/ioctl.h>
#include <unistd.h>

// TTY check
#define IsATTY isatty
#define FILENO fileno

#endif

namespace taskranger {

namespace TermUtils {

class TermData {
public:
    // Poorly named
    static bool specialSupportAnsi;
    static bool hasDevNull;

    static void initTerminal();
};

/**
 * Checks whether a given ostream is a terminal or not.
 * To be more specific, it also checks whether std::cout
 * and std::cerr have been redirected, and therefore
 * aren't "terminals". (Failing hard on the right words
 * to use here, but you hopefully get the idea :eyes:)
 *
 * Possibly broken in MinGW and/or Cygwin.
 */
bool isTerminal(std::ostream*);

/**
 * Returns whether the terminal supports ansi or not.
 * Always returns false if the provided ostream
 * isn't a terminal.
 *
 */
bool supportsAnsi(std::ostream*);

inline int getWidth() {
#ifdef WINDOZE
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

#ifdef WINDOZE

#ifndef NO_ANSI
/**
 * Checks whether or not the given HANDLE is a console output stream,
 * or if it's a file.
 *
 * Returns true if console, false otherwise
 */
bool isConsole(HANDLE file);

void setTermMode(HANDLE handle);
#endif

void enableTermColors();

#endif

} // namespace TermUtils

} // namespace taskranger

#endif
