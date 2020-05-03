#ifndef TASKRANGER_UTIL_COLORPRINTER_HPP
#define TASKRANGER_UTIL_COLORPRINTER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <any>

namespace taskranger {

// Enable forwarding of std::endl
typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
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
    bool processing256 = false;
public:

    friend ColorPrinter& operator<<(ColorPrinter& printer, ANSIFeature feature);
    friend ColorPrinter& operator<<(ColorPrinter& printer, const std::string& str);
    friend ColorPrinter& operator<<(ColorPrinter& printer, int code);
};


template <typename T>
ColorPrinter& operator<<(ColorPrinter& printer, T type) {
    std::cout << type;
    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, ANSIFeature feature);
ColorPrinter& operator<<(ColorPrinter& printer, const std::string& str);
ColorPrinter& operator<<(ColorPrinter& printer, int code);
ColorPrinter& operator<<(ColorPrinter& printer, StandardEndLine manip);

}

#endif
