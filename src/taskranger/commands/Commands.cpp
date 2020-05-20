#include "Commands.hpp"
#include "AddCommand.hpp"
#include "DoneCommand.hpp"
#include "NextCommand.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include <iostream>

namespace taskranger {

Commands::Commands() {
    // This feels inefficient as fuck.
    // TODO: benchmark.
    // Scalability beats an if-else or a switch cascade,
    // but allocating unused memory is still a way to slow
    // down the program
    this->commands["next"] = std::make_shared<NextCommand>();
    this->commands["add"] = std::make_shared<AddCommand>();
    this->commands["done"] = std::make_shared<DoneCommand>();
}

void Commands::forward(std::shared_ptr<InputData> input) {
    const std::string& subcommand = input->tokens.at("subcommand");
    if (commands.find(subcommand) != commands.end())
        commands[input->tokens.at("subcommand")]->run(input);
    else {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9
                << "Command not found: " << subcommand << ".\n"
                << ANSIFeature::CLEAR;
    }
}

} // namespace taskranger
