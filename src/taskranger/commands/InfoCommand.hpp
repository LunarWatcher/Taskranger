#pragma once

#include "Command.hpp"
#include "taskranger/data/Attribute.hpp"

namespace taskranger {

class InfoCommand : public Command {
public:
    InfoCommand() {
        this->name = "info";
        this->help = "Shows metadata for one or more tasks";
        this->usage = "taskranger info <filter>";
    }

    void run() override;
};

} // namespace taskranger
