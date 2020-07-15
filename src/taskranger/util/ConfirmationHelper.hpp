#pragma once

#include "taskranger/data/Environment.hpp"
#include "taskranger/metadata/Types.hpp"
#include <any>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace taskranger {

class ConfirmationHelper {
private:
    std::map<std::string, std::string> changes;
    Types::TaskPtrVec filteredTasks;
    std::shared_ptr<Environment> environment;

    nlohmann::json computedChanges;

    size_t bulkWarn = 0;
    bool allYes = false;

    void dumpChanges();

    void commitChanges(std::shared_ptr<Task> task);

public:
    ConfirmationHelper(std::map<std::string, std::string> newData, Types::TaskPtrVec filteredTasks);

    /**
     * Processes the list and modfies the JSON databases.
     *
     * Returns true if the databases can be commited; false otherwise.
     * While this method does modify the database objects if applicable,
     * it's up to the caller to actually commit the databases.
     * This is mainly to save processing time
     */
    size_t process();
};

} // namespace taskranger
