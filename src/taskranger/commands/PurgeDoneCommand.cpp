#include "PurgeDoneCommand.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/util/FilesystemUtil.hpp"

#include <iostream>
#include <string>

namespace taskranger {

PurgeDoneCommand::PurgeDoneCommand() {
    this->name = "purge";
    this->help = "Purges all completed tasks. This cannot be undone";
    this->usage = "taskranger purge";
}

void PurgeDoneCommand::run() {
    std::cout << "This will permanently remove all completed tasks. Are you sure? [y/n] ";

    std::string input;
    std::getline(std::cin, input);

    if (input[0] == 'y') {
        JSONDatabase("completed.json", false).purge();
        std::cout << "Completed." << std::endl;
    } else {
        std::cout << "Canceled." << std::endl;
    }
}

} // namespace taskranger
