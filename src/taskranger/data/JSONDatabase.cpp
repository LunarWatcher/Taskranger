#include "JSONDatabase.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/exceptions/Exceptions.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/FilesystemUtil.hpp"
#include <filesystem>
#include <fstream>

namespace taskranger {

JSONDatabase::JSONDatabase(const std::string& databaseName) {
    auto config = Environment::getInstance()->getConfig();
    std::string databaseFolder = config->getString("dataDir");
    if (databaseFolder.back() != '/')
        databaseFolder += "/";
    databaseFolder = FilesystemUtil::expandUserPath(databaseFolder);

    std::shared_ptr<nlohmann::json> ptr = std::make_shared<nlohmann::json>();
    std::string path = databaseFolder + databaseName;

    if (std::filesystem::exists(path)) {
        std::ifstream stream(path);
        if (!stream) {
            ColorPrinter printer;
            printer << ANSIFeature::FOREGROUND << 9 << "Failed to open a database that should exist: " << path
                    << ". Are the permissions incorrect?" << ANSIFeature::CLEAR << "\n";
            throw PermissionError("#0: failed to open existing database");
        }
        stream >> *ptr;
    }

    this->database = ptr;
    this->dbName = databaseName;
    this->dbFolder = databaseFolder;
}

void JSONDatabase::commit() {
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

} // namespace taskranger
