#include "TermUtils.hpp"
#include "ColorPrinter.hpp"
#include "taskranger/input/EnvVars.hpp"
#include <filesystem>

namespace taskranger {
bool TermUtils::TermData::specialSupportAnsi = true;
bool TermUtils::TermData::hasDevNull = false;

bool TermUtils::isTerminal(std::ostream* stream) {
    return (stream == &std::cout && IsATTY(FILENO(stdout))) || (stream == &std::cerr && IsATTY(FILENO(stderr)));
}

bool TermUtils::supportsAnsi(std::ostream* stream) {
    // Main check; if we're not in a terminal, there's a 100% chance we don't support ANSI
    // This is also in part because adding ANSI to a file, if piped properly, is really not a good idea
    // Also means any hooks or other programs trying to read Taskranger's output doesn't get useless bytes
    if (!isTerminal(stream)) {
        return false;
    }
    // There's about half a billion approaches to this, but there's one thing I'm extremely
    // brutely going to assume:
    // ANYONE with a C++17 compiler also has a modern enough terminal to support ANSI,
    // provided /dev/null exists. No, really.
    // /dev/null indicates Linux, Cygwin, or MinGW at runtime, Cygwin and MinGW both
    // support ANSI, Linux falls back to the previous statement.
    // MinGW and Cygwin should, IIRC, be able to run Windows executables without any
    // problem, which means it's perfectly possible to build Taskranger with MSVC, and
    // run in CMD, MinGW, or Cygwin.
    // This causes a few challenges, but it's really the runtime environment that matters.
    // If it has /dev/null, it's probably made by people who didn't insist on their own
    // shit system for a few years, and probably supports ANSI
    if (TermData::hasDevNull) {
        return true;
    }

    // Windoze, specifically MinGW/Cygwin. Also enables an override for, well, everything
    if (Env::getEnv("ANSICON", "nej") != "nej") {
        return true;
    }

    // This is put far down to make sure we can run cygwin and mingw tests first
    if (!TermData::specialSupportAnsi) {
        return false;
    }

    return false;
}

#ifdef WINDOZE

bool TermUtils::isConsole(HANDLE file) {
    return (GetFileType(file) & 3) == FILE_TYPE_CHAR;
}

void TermUtils::setTermMode(HANDLE handle) {
    // THE WINDOWS API REALLY LIKES TO SHOUT
    // SOMEONE SHOULD TELL THEM IT'S POSSIBLE TO TURN OFF CAPS-LOCK
    // Note to self: LPDWORD is a pointer to a DWORD
    // So is PDWORD, because Windows :facepaw:
    DWORD mode;
    if (!GetConsoleMode(handle, &mode)) {
        // We failed to get the console mode; abort.
        return;
    }
    if (!SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        auto e = GetLastError();
        if (e == STATUS_INVALID_PARAMETER) {
            // This should be the code triggered by a lack of
            // ANSI support in CMD
            TermData::specialSupportAnsi = false;
        }
    }
}

void TermUtils::enableTermColors() {
    // https://docs.microsoft.com/en-us/windows/console/getstdhandle
    HANDLE outStream = GetStdHandle(-11);
    HANDLE outErrStream = GetStdHandle(-12);

    if (isConsole(outStream)) {
        setTermMode(outStream);
    }
    if (isConsole(outErrStream)) {
        setTermMode(outErrStream);
    }
}

#endif

void TermUtils::TermData::initTerminal() {
    std::filesystem::path devnull("/dev/null");
    if (std::filesystem::exists(devnull)) {
        hasDevNull = true;
    }
#ifdef WINDOZE
    else {
        // We're Windows; we don't have /dev/null; must mean we're cmd.
        // Let's enable term colors
        enableTermColors();
    }
#endif
}

} // namespace taskranger
