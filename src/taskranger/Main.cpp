#include <iostream>
#include <vector>
#include <string>

#include "input/InputParser.hpp"
#include "taskranger/input/InputData.hpp"

int main(int argc, const char* argv[]) {

    taskranger::InputParser parser;
    std::shared_ptr<taskranger::InputData> input = parser.parseInput(argc, argv);

    std::cout << "Printing arguments.." << std::endl;
    for (auto& [k, v] : input->tokens) {
        std::cout << k << ": " << v << std::endl;
    }

    for (auto& tag : input->tags) {
        std::cout << tag << std::endl;
    }
}
