#include "Task.hpp"
#include "JSONDatabase.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/data/attributes/RecurAttribute.hpp"
#include "taskranger/util/UIDUtils.hpp"
#include <algorithm>

namespace taskranger {

Task::Task(JSONDatabase* database) : database(database) {
    auto uuid = uuid::generateUuidV4();
    nlohmann::json bootstrap = {{"uuid", uuid}};
    auto rawDatabase = database->getRawDatabase();
    rawDatabase->push_back(bootstrap);

    this->idx = rawDatabase->size() - 1;
    if (database->hasPublicIds) {
        this->taskJson["id"] = rawDatabase->size();
    }
}

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

    if (this->taskJson.contains("depends") &&
            std::get<std::string>(Environment::getInstance()->getAttribute("depends")->getMaxRepresentationForTable(
                    this->taskJson)) != " ") {
        this->taskJson["vtags"] = std::vector<std::string>{};
        this->taskJson["vtags"].push_back("+BLOCKED");
    }

    // Update recurrence tasks when they're laoded
    if (database->hasPublicIds && taskJson.contains("recur")) {
        std::static_pointer_cast<RecurAttribute>(Environment::getInstance()->getAttribute("recur"))
                ->update(*this, *database);
    }
}

bool Task::hasPublicIds() {
    return database->hasPublicIds;
}

std::string Task::getUUID() {
    return taskJson.at("uuid");
}

void Task::set(const std::string& key, const nlohmann::json& value) {
    this->taskJson[key] = value;
}

void Task::commitChanges() {
    nlohmann::json task = this->taskJson;
    task.erase("vtags");
    task.erase("id");

    this->database->getRawDatabase()->at(this->idx) = task;
}

void Task::operator>>(Task& target) const {
    static const std::vector<std::string> ignored = {"id", "uuid", "tasktype"};
    for (auto& [k, v] : this->taskJson.items()) {
        if (std::find(ignored.begin(), ignored.end(), k) == ignored.end()) {
            target.taskJson[k] = v;
        }
    }
}

} // namespace taskranger
