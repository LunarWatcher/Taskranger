#ifndef TASKRANGER_COMMANDS_VERSIONCOMMAND_HPP
#define TASKRANGER_COMMANDS_VERSIONCOMMAND_HPP

#include "Command.hpp"

namespace taskranger {

class VersionCommand : public Command {
public:
    VersionCommand();
    void run(std::shared_ptr<InputData> input) override;
};

}

#endif
