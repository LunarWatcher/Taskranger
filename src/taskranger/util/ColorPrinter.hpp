#ifndef TASKRANGER_UTIL_COLORPRINTER_HPP
#define TASKRANGER_UTIL_COLORPRINTER_HPP

#include <any>
#include <iostream>
#include <sstream>
#include <string>

namespace taskranger {

// Enable forwarding of std::endl
typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t>> CoutType;
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
    std::wostream& outputStream;

public:
    ColorPrinter() : outputStream(std::wcout) {}
    ColorPrinter(std::wostream& outputStream) : outputStream(outputStream) {}

    template <typename T>
    friend ColorPrinter& operator<<(ColorPrinter&, T);
    friend ColorPrinter& operator<<(ColorPrinter&, ANSIFeature);
    friend ColorPrinter& operator<<(ColorPrinter&, const std::string&);
    friend ColorPrinter& operator<<(ColorPrinter&, const std::wstring&);
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
ColorPrinter& operator<<(ColorPrinter& printer, const std::wstring& str);
ColorPrinter& operator<<(ColorPrinter& printer, int code);
ColorPrinter& operator<<(ColorPrinter& printer, StandardEndLine manip);

} // namespace taskranger

#endif
