#ifndef TASKRANGER_INPUT_PARSING_HPP
#define TASKRANGER_INPUT_PARSING_HPP

#include "InputData.hpp"
#include <memory>
#include <regex>
#include <string>
#include <vector>

namespace taskranger {

/**
 * Handles the input parsing core
 */
class InputParser {
private:
    /**
     * Label/filter regex.
     */
    static const inline std::regex labelRegex = std::regex{"^([0-9a-z\\.]+:[^ $].*)", std::regex_constants::icase};

public:
    InputParser();

    /**
     * Parses the command line input to usable tokens
     */
    void parseInput(int argc, const char* args[]);
};

} // namespace taskranger

#endif
