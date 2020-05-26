#ifndef TASKRANGER_COMMANDS_COMMANDS_HPP
#define TASKRANGER_COMMANDS_COMMANDS_HPP

#include "Command.hpp"
#include "taskranger/input/InputData.hpp"

namespace taskranger {

class Commands {
private:
    std::map<std::string, std::shared_ptr<Command>> commands;
    static std::shared_ptr<Commands> INSTANCE;
    Commands();
public:
    /**
     * Forward input from the caller (int main()) to a command.
     */
    void forward(std::shared_ptr<InputData> data);

    std::map<std::string, std::shared_ptr<Command>> getCommands() { return commands; }
    static std::shared_ptr<Commands> getInstance();

};


}


#endif

