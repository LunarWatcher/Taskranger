#include <string>
#include <vector>

#include "input/InputParser.hpp"
#include "taskranger/commands/Commands.hpp"
#include "taskranger/input/InputData.hpp"

int main(int argc, const char* argv[]) {

    taskranger::InputParser parser;
    std::shared_ptr<taskranger::InputData> input = parser.parseInput(argc, argv);

    taskranger::Commands commands;
    commands.forward(input);
}
