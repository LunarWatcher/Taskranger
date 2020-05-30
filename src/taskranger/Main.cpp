#include <string>
#include <vector>

#include "input/InputParser.hpp"
#include "taskranger/commands/Commands.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/input/InputData.hpp"

int main(int argc, const char* argv[]) {

    using taskranger::Environment;
    using taskranger::InputParser;

    InputParser parser;
    parser.parseInput(argc, argv);

    Environment::getInstance()->getCommands()->forward();
}
