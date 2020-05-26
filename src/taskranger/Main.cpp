#include <string>
#include <vector>

#include "input/InputParser.hpp"
#include "taskranger/commands/Commands.hpp"
#include "taskranger/input/InputData.hpp"

int main(int argc, const char* argv[]) {

    using taskranger::Commands;

    taskranger::InputParser parser;
    std::shared_ptr<taskranger::InputData> input = parser.parseInput(argc, argv);

    Commands& commands = *Commands::getInstance();
    commands.forward(input);
}
