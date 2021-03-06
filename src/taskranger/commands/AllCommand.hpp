#ifndef TASKRANGER_COMMAND_ALLCOMMAND_HPP
#define TASKRANGER_COMMAND_ALLCOMMAND_HPP

#include "Command.hpp"

namespace taskranger {

class AllCommand : public Command {
public:
    AllCommand();
    void run() override;
};

} // namespace taskranger

#endif
