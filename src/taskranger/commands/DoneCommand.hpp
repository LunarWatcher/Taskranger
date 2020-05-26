#ifndef TASKRANGER_COMMANDS_DONECOMMAND_HPP
#define TASKRANGER_COMMANDS_DONECOMMAND_HPP

#include "Command.hpp"

namespace taskranger {

class DoneCommand : public Command {
public:
    DoneCommand();
    void run(std::shared_ptr<InputData> input) override;
};

} // namespace taskranger

#endif
