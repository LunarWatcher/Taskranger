#ifndef TASKRANGER_COMMANDS_ADDCOMMAND_HPP
#define TASKRANGER_COMMANDS_ADDCOMMAND_HPP

#include "Command.hpp"
#include <iostream>

namespace taskranger {

class AddCommand : public Command {
public:
    AddCommand();
    void run() override;
};

} // namespace taskranger

#endif
