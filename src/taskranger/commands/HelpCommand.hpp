#ifndef TASKRANGER_COMMANDS_HELPCOMMAND_HPP
#define TASKRANGER_COMMANDS_HELPCOMMAND_HPP

#include "Command.hpp"
#include "taskranger/commands/Commands.hpp"
#include "taskranger/input/InputData.hpp"

namespace taskranger {

class HelpCommand : public Command {
private:
    void printCommands(Commands& commands);
public:
    HelpCommand();
    void run(std::shared_ptr<InputData> input) override;
};


}

#endif
