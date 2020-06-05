#include <iostream>
#include <string>
#include <vector>

#include "input/InputParser.hpp"
#include "taskranger/commands/Commands.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/exceptions/Exceptions.hpp"
#include "taskranger/input/InputData.hpp"

int main(int argc, const char* argv[]) {

    using taskranger::Environment;
    using taskranger::InputParser;

    InputParser parser;
    parser.parseInput(argc, argv);

    try {
        Environment::getInstance()->getCommands()->forward();
    } catch (std::string& err) {
        // It makes most sense to throw string errors. These can universally be caught here.
        // This mostly makes sense anyway, at least for errors that can't really be recovered from..
        std::cerr << err << std::endl;
        return -1;
    } catch (taskranger::PermissionError& permErr) {
        std::cerr << "Critical error: permission error:\n"
                  << permErr.what() << "\n"
                  << "This should never happen. Please make sure you have the proper file permissions, "
                  << "and open an issue on GitHub if this goes further than permissions." << std::endl;
        throw;
    }
}
