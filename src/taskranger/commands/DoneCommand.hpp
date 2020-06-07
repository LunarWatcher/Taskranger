#ifndef TASKRANGER_COMMANDS_DONECOMMAND_HPP
#define TASKRANGER_COMMANDS_DONECOMMAND_HPP

#include "Command.hpp"

namespace taskranger {

class DoneCommand : public Command {
public:
    DoneCommand();
    void run() override;
};

} // namespace taskranger

#endif
