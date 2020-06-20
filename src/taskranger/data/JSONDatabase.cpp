#include "JSONDatabase.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/exceptions/Exceptions.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/FilesystemUtil.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <filesystem>
#include <fstream>

namespace taskranger {

JSONDatabase::JSONDatabase(const std::string& databaseName, bool hasPublicIds) : hasPublicIds(hasPublicIds) {
    auto config = Environment::getInstance()->getConfig();
    std::string databaseFolder = config->getString("dataDir");

    if (databaseFolder == "") {
        throw "The database folder cannot be an empty string. If you want it to be relative, use \"dataDir\": \"./\".";
    }
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
        try {
            stream >> *ptr;
        } catch (nlohmann::json::parse_error& err) {
            std::cout << err.what() << std::endl;
            throw "Failed to load database " + databaseName +
                    " - be careful when modifying it manually. If you didn't, open an issue on GitHub";
        }
    } else if (!std::filesystem::exists(databaseFolder)) {
        std::filesystem::create_directories(databaseFolder);
    }

    this->rawDatabase = ptr;
    this->dbName = databaseName;
    this->dbFolder = databaseFolder;
    if (rawDatabase->size() == 0) {
        // The rest of the constructor is for non-empty databases only
        return;
    }
    if (!rawDatabase->is_array()) {
        throw "JSON database manually modified; the root element MUST be an array.";
    }
    for (size_t i = 0; i < rawDatabase->size(); i++) {
        this->database.push_back(std::make_shared<Task>(this, i));
    }
}

void JSONDatabase::commit() {
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
    stream << *rawDatabase;
}

} // namespace taskranger
