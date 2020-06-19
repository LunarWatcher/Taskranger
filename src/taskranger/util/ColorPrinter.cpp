#include "ColorPrinter.hpp"
#include <codecvt>
#include <locale>

namespace taskranger {

ColorPrinter& operator<<(ColorPrinter& printer, ANSIFeature feature) {
    if (printer.processing256)
        throw std::runtime_error("Cannot print a new ANSI feature while attempting to print a color");

    printer.outputStream << "\033[" << int(feature);
    if (feature == ANSIFeature::FOREGROUND || feature == ANSIFeature::BACKGROUND) {
        printer.processing256 = true;
        printer.outputStream << ";5;";
    } else {
        printer.outputStream << "m";
    }

    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, const std::string& str) {
    printer.outputStream << str;
    if (printer.processing256) {
        printer.processing256 = false;
        printer.outputStream << "m";
    }
    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, int code) {
    printer.outputStream << code;
    if (printer.processing256) {
        printer.processing256 = false;
        printer.outputStream << "m";
    }
    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, StandardEndLine manip) {
    manip(printer.outputStream);
    return printer;
}

} // namespace taskranger
