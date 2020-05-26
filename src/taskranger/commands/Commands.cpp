#include "Commands.hpp"
#include "AddCommand.hpp"
#include "DoneCommand.hpp"
#include "NextCommand.hpp"
#include "taskranger/commands/AllCommand.hpp"
#include "taskranger/commands/HelpCommand.hpp"
#include "taskranger/commands/VersionCommand.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include <iostream>

namespace taskranger {

std::shared_ptr<Commands> Commands::INSTANCE = nullptr;

Commands::Commands() {
    // This feels inefficient as fuck.
    // TODO: benchmark.
    // Scalability beats an if-else or a switch cascade,
    // but allocating unused memory is still a way to slow
    // down the program
    // Listing
    this->commands["next"] = std::make_shared<NextCommand>();
    this->commands["all"] = std::make_shared<AllCommand>();
    // RW
    this->commands["add"] = std::make_shared<AddCommand>();
    this->commands["done"] = std::make_shared<DoneCommand>();

    // Meta
    this->commands["version"] = std::make_shared<VersionCommand>();
    this->commands["help"] = std::make_shared<HelpCommand>();
}

void Commands::forward(std::shared_ptr<InputData> input) {
    const std::string& subcommand = input->tokens.at("subcommand");
    if (commands.find(subcommand) != commands.end())
        commands[input->tokens.at("subcommand")]->run(input);
    else {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9 << "Command not found: " << subcommand << ".\n" << ANSIFeature::CLEAR;
    }
}

std::shared_ptr<Commands> Commands::getInstance() {
    if (Commands::INSTANCE == nullptr)
        Commands::INSTANCE = std::shared_ptr<Commands>(new Commands());
    return Commands::INSTANCE;
}

} // namespace taskranger
