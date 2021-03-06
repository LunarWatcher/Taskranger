#ifndef TASKRANGER_DATA_JSONDATABASE_HPP
#define TASKRANGER_DATA_JSONDATABASE_HPP

#include "nlohmann/json.hpp"
#include "taskranger/data/Task.hpp"
#include "taskranger/util/CompatUtil.hpp"
#include <any>
#include <functional>

namespace taskranger {

/**
 * This may have performance issues over time
 */
class JSONDatabase {
private:
    std::string dbName;
    std::string dbFolder;
    std::shared_ptr<nlohmann::json> rawDatabase;
    std::vector<std::shared_ptr<Task>> database;

public:
    const bool hasPublicIds;
    bool demoMode = false;

    JSONDatabase() = delete;
    JSONDatabase(const std::string& databaseName, bool hasPublicIds);

    void commit();

    std::vector<std::shared_ptr<Task>>& getDatabase() {
        return database;
    }

    std::shared_ptr<nlohmann::json> getRawDatabase() {
        return rawDatabase;
    }

    size_t size() {
        return rawDatabase->size();
    }

    void purge();

    std::shared_ptr<Task> contains(const std::string& fieldName, const std::string& value);
    bool containsAny(const std::string& fieldName, const std::vector<std::string>& allowedValues);

    void initVTags() {
        for (auto task : this->database) {
            task->initVTags();
        }
    }
};

} // namespace taskranger

#endif
