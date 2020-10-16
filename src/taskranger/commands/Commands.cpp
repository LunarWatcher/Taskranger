#include "Commands.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include <iostream>

// Command includes
#include "AddCommand.hpp"
#include "AllCommand.hpp"
#include "DoneCommand.hpp"
#include "HelpCommand.hpp"
#include "InfoCommand.hpp"
#include "ModifyCommand.hpp"
#include "NextCommand.hpp"
#include "VersionCommand.hpp"

namespace taskranger {

Commands::Commands() {
    // This feels inefficient as fuck.
    // TODO: benchmark.
    // Scalability beats an if-else or a switch cascade,
    // but allocating unused memory is still a way to slow
    // down the program
    // Listing
    this->commands["next"] = std::make_shared<NextCommand>();
    this->commands["all"] = std::make_shared<AllCommand>();
    this->commands["info"] = std::make_shared<InfoCommand>();

    // RW
    this->commands["add"] = std::make_shared<AddCommand>();
    this->commands["done"] = std::make_shared<DoneCommand>();
    this->commands["modify"] = std::make_shared<ModifyCommand>();

    // Meta
    this->commands["version"] = std::make_shared<VersionCommand>();
    this->commands["help"] = std::make_shared<HelpCommand>();
}

void Commands::forward() {
    auto input = Environment::getInstance()->getInputData();
    const std::string& subcommand = input->data.at("subcommand");
    if (commands.find(subcommand) != commands.end())
        commands[input->data.at("subcommand")]->run();
    else {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9 << "Command not found: " << subcommand << ".\n" << ANSIFeature::CLEAR;
    }
}

} // namespace taskranger
