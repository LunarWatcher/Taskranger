#include "NextCommand.hpp"
#include <iostream>
#include "tabulate/table.hpp"
#include "taskranger/util/ColorPrinter.hpp"

namespace taskranger {

void NextCommand::run(std::shared_ptr<InputData> input) {
    tabulate::Table taskTable;
    taskTable.add_row({"Label", "Value"});
    for (auto& [k, v] : input->tokens) {
        taskTable.add_row({k, v});
    }

    taskTable.format()
        .border_color(tabulate::Color::yellow)
        .multi_byte_characters(true)
        .border_top("─")
        .border_bottom("─")
        .border_left("│")
        .border_right("│")
        .width(80);
    ColorPrinter printer; 
    printer << "I am once again bothering my next command with abusive tests.\n" 
        << "Why, you ask? " 
        << ANSIFeature::BLINK << "BECAUSE I CAN! :D\n" << ANSIFeature::CLEAR
        << ANSIFeature::BOLD << "I've added a few ANSI codes. " 
        << ANSIFeature::FOREGROUND << 121 << "But notably 256 color. True color was too hard "
                                             "to implement without using classes, and that's ugly."
        << ANSIFeature::CLEAR << std::endl;

    std::cout << taskTable << std::endl;
}

}
