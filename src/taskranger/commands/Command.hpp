#ifndef TASKRANGER_COMMANDS_COMMAND_HPP
#define TASKRANGER_COMMANDS_COMMAND_HPP

#include "taskranger/input/InputData.hpp"
#include <memory>
#include <string>

namespace taskranger {

class Command {
protected:
    // Metadata
    std::string name;
    std::string help;
    std::string usage;

public:
    virtual ~Command() = default;

    virtual void run() = 0;

    const std::string& getName() {
        return this->name;
    }

    const std::string& getHelp() {
        return this->help;
    }

    const std::string& getUsage() {
        return usage;
    }
};

} // namespace taskranger

#endif
