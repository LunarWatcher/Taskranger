#ifndef TASKRANGER_DATA_JSONDATABASE_HPP
#define TASKRANGER_DATA_JSONDATABASE_HPP

#include "nlohmann/json.hpp"
#include <any>

namespace taskranger {

/**
 * This may have performance issues over time
 */
class JSONDatabase {
private:
    std::string dbName;
    std::string dbFolder;
    std::shared_ptr<nlohmann::json> database;

public:
#ifdef UNITTEST
    bool demoMode = false;
#endif

    JSONDatabase() = delete;
    JSONDatabase(const std::string& databaseName);

    void commit();

    std::shared_ptr<nlohmann::json> getDatabase() {
        return database;
    }

    size_t size() {
        return database->size();
    }
};

} // namespace taskranger

#endif
