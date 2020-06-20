#pragma once

#include "taskranger/commands/Commands.hpp"
#include "taskranger/config/ConfigHelper.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/data/TaskInfo.hpp"
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

    /**
     * Internal map of databases.
     * Used to avoid unnecessary reads.
     */
    std::map<std::string, std::shared_ptr<JSONDatabase>> databases;
    std::map<std::string, std::shared_ptr<Attribute>> attributes;

public:
    Environment();

    std::shared_ptr<InputData> getInputData();
    std::shared_ptr<Config> getConfig();
    std::shared_ptr<Commands> getCommands();

    /**
     * Returns an attribute.
     *
     * This uses a local cache. If the attribute has been loaded, return that.
     * If it hasn't been loaded, try loading it. If the attribute doesn't exist,
     * return nullptr.
     */
    std::shared_ptr<Attribute> getAttribute(const std::string& attribName);
    std::shared_ptr<JSONDatabase> getDatabase(const std::string& dbName, bool hasPublicIds);

    static std::shared_ptr<Environment> getInstance();

#ifdef UNITTEST
    /**
     * This is a special unit test function that resets the entire instance.
     * This should NEVER be used outside the scope of unit testing.
     * It has also been macro-protected to avoid incorrect use.
     */
#ifdef TASKRANGER_NON_UNIT_TEST
#error "NO! Read the god damn comment block!"
#endif
    static std::shared_ptr<Environment> createInstance() {
        INSTANCE = std::make_shared<Environment>();
        return INSTANCE;
    }
#endif
};

} // namespace taskranger
