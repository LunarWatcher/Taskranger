#ifndef TASKRANGER_COMMANDS_COMMANDS_HPP
#define TASKRANGER_COMMANDS_COMMANDS_HPP

#include "Command.hpp"
#include "taskranger/input/InputData.hpp"

namespace taskranger {

class Environment;
class Commands {
private:
    std::map<std::string, std::shared_ptr<Command>> commands;

public:
    Commands();
    /**
     * Forward input from the caller (int main()) to a command.
     */
    void forward();

    std::map<std::string, std::shared_ptr<Command>> getCommands() {
        return commands;
    }

    /**
     * This is mainly intended for unit tests. It's protected by the UNITTEST
     * macro because it shouldn't take up space in the release binary.
     */
    std::shared_ptr<Command> getCommand(const std::string& command) {
        return commands.at(command);
    }
};

} // namespace taskranger

#endif
