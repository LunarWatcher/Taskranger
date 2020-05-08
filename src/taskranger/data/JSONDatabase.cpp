#include "JSONDatabase.hpp"
#include <fstream>
#include <filesystem>
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/FilesystemUtil.hpp"

namespace taskranger {

JSONDatabase::JSONDatabase(const std::string& databaseName) {
    std::string databaseFolder = "~/.taskranger"; // TODO: fix and make portable (and add config)
    if (databaseFolder.back() != '/') databaseFolder += "/";
    databaseFolder = FilesystemUtil::expandUserPath(databaseFolder);

    std::shared_ptr<nlohmann::json> ptr = std::make_shared<nlohmann::json>();
    std::string path = databaseFolder + databaseName;

    if (std::filesystem::exists(path)) {
        std::ifstream stream(path);
        if (!stream) {
            ColorPrinter printer;
            printer
                << ANSIFeature::FOREGROUND << 9
                << "Failed to open a database that should exist: "
                << path << ". Are the permissions incorrect?" << ANSIFeature::CLEAR
                << std::endl;
            return;
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

    if(!stream) {
        ColorPrinter printer;
        printer
            << ANSIFeature::FOREGROUND << 9
            << "Failed to open the file. Are the permissions correct?"
            << ANSIFeature::CLEAR << std::endl;
        return;
    }
    stream << *database;
}

} // namespace taskranger
