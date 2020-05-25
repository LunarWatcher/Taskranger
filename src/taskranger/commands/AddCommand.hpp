#ifndef TASKRANGER_COMMANDS_ADDCOMMAND_HPP
#define TASKRANGER_COMMANDS_ADDCOMMAND_HPP

#include <iostream>
#include "Command.hpp"

namespace taskranger {

class AddCommand : public Command {
public:
    AddCommand();
    void run(std::shared_ptr<InputData> input) override;
};

}

#endif
