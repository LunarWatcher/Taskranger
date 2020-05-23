#include "FilesystemUtil.hpp"
#include "StrUtil.hpp"
#include <regex>
#include <stdexcept>

namespace taskranger {

std::string FilesystemUtil::joinPath(const std::string& a, const std::string& b) {
    if (isSeparator(a.back()) || isSeparator(b.front()))
        return a + b;
    return a + "/" + b;
}

bool FilesystemUtil::isSeparator(const char x) { return x == '/' || x == '\\'; }

std::string FilesystemUtil::expandUserPath(const std::string& inputPath) {

    // Convert all backslashes to forward slashes for processing (fuck you Windows)
    std::string rawPath = std::regex_replace(inputPath, std::regex("\\\\"), "/");

    // In order to universally support paths without doing a hacky if-check elsewhere,
    // this just returns the path itself if it isn't a home path.
    // Other paths should work themselves out
    if (rawPath.at(0) != '~') return rawPath;

    std::optional<std::string> username;
    std::string remainingPath;

    // The next part of the code is used to parse off the username,
    // and grab the rest of the path. This is necessary to reconstruct
    // the path afterwards.
    std::string mod = rawPath.substr(1);
    std::vector<std::string> pathSplit = Util::splitString(mod, "/", 1);

    //  Parse off the username
    if (rawPath.length() >= 2 && !isSeparator(rawPath.at(1))) {
        // ~username
        // ~username/path
        username = pathSplit.at(0);
        remainingPath = pathSplit.at(1);
    } else if (rawPath.find('/') != std::string::npos) {
        // The path contains a slash.
        if (pathSplit.size() == 1) {
            throw std::runtime_error("There is absolutely no reason for this to trigger, at all. If it is, hit Olivia with a rolled up newspaper c:");
        }

        remainingPath = pathSplit.at(1);
    }

    // We've by now found a username or not. This means we can actually expand
    // the path.

    std::string homePath = "";
#if defined(_WIN32) || defined(_WIN64)

    if (!username.has_value()) {
        auto home = getenv("HOME");
        if (!home) {
            auto userProfile = getenv("USERPROFILE");
            std::cout << userProfile << std::endl;
            if (!userProfile) {
                auto homeDrive = getenv("HOMEDRIVE");
                std::cout << homeDrive << std::endl;
                if (!homeDrive)
                    homeDrive = ""; // Let the OS resolve the path
                auto envHomePath = getenv("HOMEPATH");
                std::cout << envHomePath << std::endl;
                if (!envHomePath) {
                    ColorPrinter printer;
                    printer << ANSIFeature::FOREGROUND << 9
                        << "Unable to find %HOMEPATH%. Specify the path explicitly instead."
                        << ANSIFeature::CLEAR
                        << "\n";
                    return "";
                }
                homePath = std::string(homeDrive) + std::string(envHomePath);
            } else
                homePath = userProfile;
        } else
            homePath = home;
    } else {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9
            << "This doesn't work."
            << ANSIFeature::CLEAR
            << " Due to Windows having a very limited API for expanding user paths, and it relies on environment "
            << "variables and assumptions, me (the developer), has decided to not implement ~user expansion on Windows. "
            << "I cannot easily test it, nor can I find any reassuring information for a universal pattern I can use. "
            << "Replace your path with an absolute path instead. An implementation for this feature may be available in the future.\n";
    }
#else
    /*
     The unixes are more complicated, but the API should be universal and make
     this a lot easier.

     The idea is checking for HOME if we're looking for the current user.
     If we can't find the home variable, fall back to a UNIX-specific^1
     function that retrieves the path, along with a few other details.
     see getpwnam(3) for more info on the functions, and passwd(5)
     for details on the struct.

     If a username has been supplied (~username/), getpwnam is used instead
     of getpwuid. This returns the same type of struct as getpwuid().

     The bonus with the UNIX approach is that it requires the user to exist,
     where as Windows for non-existent users with a custom specified path should
     in theory cause a bad path. This is on the user, however.

     ^1: untested on mac and other UNIX-based systems. Only verified on Linux.
    */
    struct passwd* passwdPtr = nullptr;

    if (!username.has_value()) {
        auto home = std::getenv("HOME");
        if (home != nullptr) {
            // okay, we good.
            // This path leads to the current user home.
            // There are some caveats, and it's not always set, however. For this reason, there's a fallback clause.
            homePath = home;
        } else {
            // Fallback; HOME isn't anways defined on UNIX-based systems.
            // This is a complicated clusterfuck in terms of when it's there
            // and when it isn't, so this is implemented here as a universal
            // fallback. This should work:tm:
            passwdPtr = getpwuid(getuid());
        }
    } else {
        auto& name = *username;
        passwdPtr = getpwnam(name.c_str());
    }

    if (passwdPtr == nullptr && homePath == "") {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9 << "Failed to expand the user path for " << rawPath
                << ANSIFeature::CLEAR << ". The system seems to think you don't exist. "
                << "Please specify the path to use - don't abbreviate it with ~.\n";
        return "";
    } else if (homePath == "")
        homePath = passwdPtr->pw_dir;
#endif
    return joinPath(homePath, remainingPath);
}

} // namespace taskranger
