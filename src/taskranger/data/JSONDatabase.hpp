#ifndef TASKRANGER_DATA_JSONDATABASE_HPP
#define TASKRANGER_DATA_JSONDATABASE_HPP

#include "nlohmann/json.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include <any>
#include <filesystem>
#include <fstream>

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
    /**
     * Optional flag that prevents the database from writing during a unit test
     */
    bool demoMode = false;
#endif

    JSONDatabase() = delete;
    JSONDatabase(const std::string& databaseName);

    void commit() {
#ifdef UNITTEST
        if (demoMode) {
            return;
        }
#endif

        if (!std::filesystem::exists(this->dbFolder))
            std::filesystem::create_directories(this->dbFolder);
        std::ofstream stream(this->dbFolder + this->dbName);

        if (!stream) {
            ColorPrinter printer;
            printer << ANSIFeature::FOREGROUND << 9 << "Failed to open the file. Are the permissions correct?"
                    << ANSIFeature::CLEAR << "\n";
            return;
        }
        stream << *database;
    }

    std::shared_ptr<nlohmann::json> getDatabase() {
        return database;
    }

    size_t size() {
        return database->size();
    }
};

} // namespace taskranger

#endif
