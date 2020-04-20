#ifndef TASKRANGER_INPUT_PARSING_HPP
#define TASKRANGER_INPUT_PARSING_HPP

#include <string>
#include <vector>
#include <memory>
#include <regex>
#include "InputData.hpp"

namespace taskranger {

/**
 * Handles the input parsing core
 */
class InputParser {
private:
    static const inline std::regex labelRegex = std::regex{"^([a-z]+:[^ $].*?)", std::regex_constants::icase };
public:
    InputParser();

    /**
     * Parses the command line input to usable tokens
     */
    std::shared_ptr<InputData> parseInput(int argc, const char* args[]);

};

}

#endif
