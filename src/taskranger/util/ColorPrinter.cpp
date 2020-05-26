#include "ColorPrinter.hpp"

namespace taskranger {

ColorPrinter& operator<<(ColorPrinter& printer, ANSIFeature feature) {
    if (printer.processing256)
        throw std::runtime_error("Cannot print a new ANSI feature while attempting to print a color");

    std::cout << "\033[" << int(feature);
    if (feature == ANSIFeature::FOREGROUND || feature == ANSIFeature::BACKGROUND) {
        printer.processing256 = true;
        std::cout << ";5;";
    } else {
        std::cout << "m";
    }

    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, const std::string& str) {
    std::cout << str;
    if (printer.processing256) {
        printer.processing256 = false;
        std::cout << "m";
    }
    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, int code) {
    std::cout << code;
    if (printer.processing256) {
        printer.processing256 = false;
        std::cout << "m";
    }
    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, StandardEndLine manip) {
    manip(std::cout);
    return printer;
}

} // namespace taskranger
