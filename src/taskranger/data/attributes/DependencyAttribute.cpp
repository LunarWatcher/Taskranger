#include "DependencyAttribute.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/metadata/Types.hpp"
#include "unicode/urename.h"

namespace taskranger {

void DependencyAttribute::modify(nlohmann::json& task, const std::string& key, const std::string& input) {

    Attribute::modify(task, key, input);
    auto& active = *Environment::getInstance()->getDatabase("active.json", true);

    std::vector<std::string> uuids;

    auto ids = StrUtil::splitString(input, ",");

    for (auto& id : ids) {
        if (id.length() == 0) {
            // Skip empty strings to avoid weirdness
            continue;
        }
        unsigned long long value = 0;
        size_t convEndPos;
        try {
            value = std::stoull(id, &convEndPos);
        } catch (std::invalid_argument&) { convEndPos = 0; }
        if (convEndPos != id.length()) {
            // it isn't a number; let's check if it exists as a UUID
            if (active.contains("uuid", id)) {
                uuids.push_back(id);
            } else {
                std::cout << "Cannot find uuid=" << id << ". Skipping...\n";
                continue;
            }

        } else {

            if (active.size() < (value == 0 ? 0 : value - 1)) {
                std::cout << "Cannot find id=" << id << ". Skipping...\n";
            } else {
                uuids.push_back(active.getRawDatabase()->at(value == 0 ? 0 : value - 1).at("uuid").get<std::string>());
            }
        }
    }

    if (uuids.size() == 0) {
        return;
    }

    task["depends"] = uuids;
}

Types::TableRow DependencyAttribute::getMinimalRepresentationForTable(const nlohmann::json& task) {
    return this->getMaxRepresentationForTable(task);
}

Types::TableRow DependencyAttribute::getMaxRepresentationForTable(const nlohmann::json& task) {
    auto depends = task.find("depends");
    if (depends == task.end()) {
        return std::string{" "};
    }
    std::string concat = " ";
    auto& active = *Environment::getInstance()->getDatabase("active.json", true);
    for (auto& uuid : depends.value()) {
        if (auto task = active.contains("uuid", uuid.get<std::string>())) {
            concat += (concat.size() != 0 ? ", " : "") + std::to_string(task->getId());
        }
    }

    return concat;
}

} // namespace taskranger
