#include "ColorPrinter.hpp"
#include "printable/utils/TermUtils.hpp"
#include <codecvt>
#include <locale>

namespace taskranger {

ColorPrinter::ColorPrinter(std::ostream& outputStream) : outputStream(outputStream) {
    supportsAnsi = printable::TermUtils::supportsAnsi(&outputStream);
}

ColorPrinter& operator<<(ColorPrinter& printer, ANSIFeature feature) {
    if (!printer.supportsAnsi) {
        // While we're dropping the print itself, we can't drop this part.
        // If we do, we get a number printed out, which isn't what we want
        printer.processing256 = true;
        // Silently drop print
        return printer;
    }
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

    if (printer.shouldReturnEarly()) {
        return printer;
    }
    printer.outputStream << str;
    if (printer.processing256) {
        printer.processing256 = false;

        printer.outputStream << "m";
    }
    return printer;
}

ColorPrinter& operator<<(ColorPrinter& printer, int code) {
    if (printer.shouldReturnEarly()) {
        return printer;
    }
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
