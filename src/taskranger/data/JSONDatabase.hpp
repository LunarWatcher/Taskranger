#ifndef TASKRANGER_DATA_JSONDATABASE_HPP
#define TASKRANGER_DATA_JSONDATABASE_HPP

#include "nlohmann/json.hpp"
#include <any>

namespace taskranger {

/**
 * This may have performance issues over time
 *
 * TODO for later: improve performance to make this blazingly fast.
 * An idea behind splitting it up alone is to reduce the size of what
 * it has to load to work properly..
 */
class JSONDatabase {
private:
    std::string dbName;
    std::string dbFolder;
    std::shared_ptr<nlohmann::json> database;
public:
    JSONDatabase(const std::string& databaseName);

    void commit(); 

    std::shared_ptr<nlohmann::json> getDatabase() { return database; }

};

} // namespace taskranger

#endif
