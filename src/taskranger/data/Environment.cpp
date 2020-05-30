#include "Environment.hpp"

namespace taskranger {

std::shared_ptr<Environment> Environment::INSTANCE = nullptr;

Environment::Environment() {
    this->inputData = std::make_shared<InputData>();
    this->config = std::make_shared<Config>();
    this->commands = std::make_shared<Commands>();
}

std::shared_ptr<InputData> Environment::getInputData() {
    return inputData;
}

std::shared_ptr<Config> Environment::getConfig() {
    config->ensureLoaded();
    return config;
}

std::shared_ptr<Commands> Environment::getCommands() {
    return commands;
}

std::shared_ptr<Environment> Environment::getInstance() {
    if (INSTANCE == nullptr) {
        INSTANCE = std::make_shared<Environment>();
    }
    return INSTANCE;
}

} // namespace taskranger
