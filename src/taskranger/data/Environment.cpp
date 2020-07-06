#include "Environment.hpp"

namespace taskranger {

std::shared_ptr<Environment> Environment::INSTANCE = nullptr;

Environment::Environment() {
    this->inputData = std::make_shared<InputData>();
    this->config = std::make_shared<Config>();
    this->commands = std::make_shared<Commands>();
    this->databases = {};
    this->attributes = {};
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

std::shared_ptr<Attribute> Environment::getAttribute(const std::string& rawAttribName) {
    std::string attribName = rawAttribName;
    if (attribName.find(".") != std::string::npos) {
        attribName = StrUtil::splitString(attribName, ".", 1).at(0);
    }
    if (attributes.find(attribName) == attributes.end()) {
        auto tryGet = Attribute::createAttrib(attribName);
        if (!tryGet) {
            return nullptr;
        }
        attributes[attribName] = tryGet;
        return tryGet;
    }
    return attributes.at(attribName);
}

std::shared_ptr<Environment> Environment::getInstance(bool force) {
    if (INSTANCE == nullptr || force) {
        INSTANCE = std::make_shared<Environment>();
    }
    return INSTANCE;
}

std::shared_ptr<JSONDatabase> Environment::getDatabase(const std::string& dbName, bool hasPublicIds) {
    auto it = this->databases.find(dbName);
    if (it == this->databases.end()) {

        auto db = std::make_shared<JSONDatabase>(dbName, hasPublicIds);
        this->databases[dbName] = db;
        return db;
    }
    return it->second;
}

} // namespace taskranger
