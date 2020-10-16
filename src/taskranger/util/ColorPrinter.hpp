#ifndef TASKRANGER_UTIL_COLORPRINTER_HPP
#define TASKRANGER_UTIL_COLORPRINTER_HPP

#include <any>
#include <iostream>
#include <sstream>
#include <string>

namespace taskranger {

// Enable forwarding of std::endl
typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
typedef CoutType& (*StandardEndLine)(CoutType&);

/**
 * Enum for properly mapping the ANSI features.
 */
enum class ANSIFeature {
    CLEAR = 0,
    BOLD = 1,
    ITALIC = 3, // According to Wikipedia, this is some times treated as inverse
    UNDERLINE = 4,
    BLINK = 5,
    STRIKE = 9,
    FOREGROUND = 38,
    BACKGROUND = 48
};

class ColorPrinter {
private:
    /**
     * Cache for whether or not the provided output stream supports ansi.
     * This variable is a reflection of TermUtils::supportsAnsi(stream),
     * and is calculated in the constructor.
     *
     * Note that certain variable changes involving processing256 will
     * still be computed in order to drop all ANSI-related input.
     * In the event this variable is false, the processing256 variable
     * is a sign to drop the next variable instead of using it for
     * something useful.
     */
    bool supportsAnsi;

    /**
     * Internal variable; used to tell certain operator<<
     * functions that they're about to get ANSI input or ANSI-related
     * input that requires special processing.
     *
     * If supportsAnsi = false, this doubles down as a flag to discard
     * certain parts of the input.
     */
    bool processing256 = false;
    std::ostream& outputStream;

    bool shouldReturnEarly() {
        if (!supportsAnsi && processing256) {
            processing256 = false;
            return true;
        }
        return false;
    }

public:
    ColorPrinter(std::ostream& outputStream);
    ColorPrinter() : ColorPrinter(std::cout) {}

    template <typename T>
    friend ColorPrinter& operator<<(ColorPrinter&, T);
    friend ColorPrinter& operator<<(ColorPrinter&, ANSIFeature);
    friend ColorPrinter& operator<<(ColorPrinter&, const std::string&);
    friend ColorPrinter& operator<<(ColorPrinter&, int);
    friend ColorPrinter& operator<<(ColorPrinter&, StandardEndLine manip);
};

template <typename T>
ColorPrinter& operator<<(ColorPrinter& printer, T type) {
    printer.outputStream << type;
    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, ANSIFeature feature);
ColorPrinter& operator<<(ColorPrinter& printer, const std::string& str);
ColorPrinter& operator<<(ColorPrinter& printer, int code);
ColorPrinter& operator<<(ColorPrinter& printer, StandardEndLine manip);

} // namespace taskranger

#endif
