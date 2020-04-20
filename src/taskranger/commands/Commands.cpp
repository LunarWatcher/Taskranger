#include "Commands.hpp"
#include "NextCommand.hpp"
#include <iostream>

namespace taskranger {

Commands::Commands() {
    this->commands["next"] = std::make_shared<NextCommand>();
}

void Commands::forward(std::shared_ptr<InputData> input) {
    const std::string& subcommand = input->tokens.at("subcommand");
    if (commands.find(subcommand) != commands.end())
        commands[input->tokens.at("subcommand")]->run(input);
    else {
        std::cout << "\033[31mCommand not found: " << subcommand << "\033[0m" << std::endl;
    }
}

}
