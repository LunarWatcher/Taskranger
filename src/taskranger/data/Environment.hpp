#pragma once

#include "taskranger/commands/Commands.hpp"
#include "taskranger/config/ConfigHelper.hpp"
#include "taskranger/input/InputData.hpp"
#include <memory>

namespace taskranger {

/**
 * Contains information about the environment, and the input.
 * This is a singleton class with a bypass for unit tests.
 */
class Environment {
private:
    static std::shared_ptr<Environment> INSTANCE;
    std::shared_ptr<InputData> inputData;
    std::shared_ptr<Config> config;
    std::shared_ptr<Commands> commands;

public:
    Environment();

    std::shared_ptr<InputData> getInputData();
    std::shared_ptr<Config> getConfig();
    std::shared_ptr<Commands> getCommands();

    static std::shared_ptr<Environment> getInstance();

#ifdef UNITTEST
    std::shared_ptr<Environment> createInstance() {
        this->INSTANCE = std::make_shared<Environment>();
        return this->INSTANCE;
    }
#endif
};

} // namespace taskranger
