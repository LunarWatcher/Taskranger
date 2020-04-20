#ifndef TASKRANGER_COMMANDS_NEXTCOMMAND_HPP
#define TASKRANGER_COMMANDS_NEXTCOMMAND_HPP
#include "Command.hpp"
#include "tabulate/table.hpp"

namespace taskranger {

class NextCommand : public Command {
public:
    NextCommand() : Command("next") {

    }
    ~NextCommand() = default;
    virtual void run(std::shared_ptr<InputData> input) override;
};

}

#endif
