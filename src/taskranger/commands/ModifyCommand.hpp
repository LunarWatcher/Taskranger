#pragma once

#include "Command.hpp"

namespace taskranger {

class ModifyCommand : public Command {
public:
    ModifyCommand() {
        this->name = "modify";
        this->help = "Used to modify one or more tasks by their ID";
        this->usage = "taskranger modify ids:<affected tasks> <modifications>";
    }
    virtual void run() override;
};

} // namespace taskranger
