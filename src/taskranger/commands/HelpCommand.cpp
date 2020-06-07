#include "HelpCommand.hpp"
#include "Commands.hpp"
#include "tabulate/table.hpp"
#include "taskranger/data/Environment.hpp"
#include <iostream>
#include <memory>

namespace taskranger {

HelpCommand::HelpCommand() {
    this->name = "help";
    this->help = "Shows a help message.";
    this->usage = "taskranger help";
}

void HelpCommand::printCommands(Commands& commands) {
    std::cout << "Commands:\n";
    for (auto& [_, command] : commands.getCommands()) {
        std::cout << "  " << command->getName() << "\n    Description: " << command->getHelp()
                  << "\n    Usage: " << command->getUsage() << "\n";
    }
}

void HelpCommand::run() {
    // Used to retrieve command help
    Commands& commands = *Environment::getInstance()->getCommands();
    this->printCommands(commands);
    // help.txt contains a raw string. Due to various annoying
    // issues with C++, adding the include statement to a raw
    // string here to make this statement slightly less ugly
    // isn't supported.
    //
    // Some resources on this:
    //   https://stackoverflow.com/a/25021520/6296561
    //   https://stackoverflow.com/q/37622767/6296561
    std::cout <<
#include "taskranger/raw/help.txt"
              << std::endl;
}

} // namespace taskranger
