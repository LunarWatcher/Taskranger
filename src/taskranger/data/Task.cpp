#include "Task.hpp"
#include "JSONDatabase.hpp"

namespace taskranger {

Task::Task(JSONDatabase* database, unsigned long long idx) : database(database), idx(idx) {}

unsigned long long Task::getId() {
    return database->hasPublicIds ? idx + 1 : 0;
}

unsigned long long Task::getId() const {
    return database->hasPublicIds ? idx + 1 : 0;
}

const nlohmann::json Task::getTaskJson() const {
    nlohmann::json data = database->getRawDatabase()->at(this->idx);
    // Append internal fields
    auto id = this->getId();
    if (id != 0) {
        data["id"] = id;
    } else {
        auto itr = data.find("id");
        if (itr != data.end()) {
            data.erase(itr);
        }
    }

    return data;
}

bool Task::hasPublicIds() {
    return database->hasPublicIds;
}

std::string Task::getUUID() {
    return database->getRawDatabase()->at(this->idx).at("uuid");
}

} // namespace taskranger
