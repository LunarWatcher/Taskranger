#ifndef TASKRANGER_COMMANDS_VERSIONCOMMAND_HPP
#define TASKRANGER_COMMANDS_VERSIONCOMMAND_HPP

#include "Command.hpp"

namespace taskranger {

class VersionCommand : public Command {
public:
    VersionCommand();
    void run() override;
};

} // namespace taskranger

#endif
