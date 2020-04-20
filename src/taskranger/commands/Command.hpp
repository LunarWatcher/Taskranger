#ifndef TASKRANGER_COMMANDS_COMMAND_HPP
#define TASKRANGER_COMMANDS_COMMAND_HPP

#include "taskranger/input/InputData.hpp"
#include <memory>
#include <string>

namespace taskranger {

class Command {
protected:
    std::string name;
public:
    Command(const std::string& name) : name(name) {}
    virtual ~Command() = default;

    virtual void run(std::shared_ptr<InputData> data) = 0;

    const std::string& getName() {
        return this->name;
    }

};

}

#endif
