#include "Task.hpp"
#include "JSONDatabase.hpp"
#include "taskranger/data/Environment.hpp"
#include <algorithm>

namespace taskranger {

Task::Task(JSONDatabase* database, unsigned long long idx)
        : database(database), taskJson(database->getRawDatabase()->at(idx)), idx(idx) {
    if (database->hasPublicIds) {
        this->taskJson["id"] = idx + 1;
    }
}

unsigned long long Task::getId() {
    return database->hasPublicIds ? idx + 1 : 0;
}

unsigned long long Task::getId() const {
    return database->hasPublicIds ? idx + 1 : 0;
}

const nlohmann::json& Task::getTaskJson() const {
    return this->taskJson;
}

void Task::initVTags() {

    // Compute virtual tags

    if (std::get<std::string>(Environment::getInstance()->getAttribute("depends")->getMaxRepresentationForTable(
                this->taskJson)) != " ") {
        this->taskJson["vtags"] = {};
        this->taskJson["vtags"].push_back("+BLOCKED");
    }
}

bool Task::hasPublicIds() {
    return database->hasPublicIds;
}

std::string Task::getUUID() {
    return taskJson.at("uuid");
}

} // namespace taskranger
