#ifndef TASKRANGER_COMMANDS_NEXTCOMMAND_HPP
#define TASKRANGER_COMMANDS_NEXTCOMMAND_HPP
#include "Command.hpp"
#include "tabulate/table.hpp"

namespace taskranger {

class NextCommand : public Command {
public:
    NextCommand();
    ~NextCommand() = default;
    void run() override;
};

} // namespace taskranger

#endif
