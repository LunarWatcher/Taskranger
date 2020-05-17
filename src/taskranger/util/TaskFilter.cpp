#include "TaskFilter.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <algorithm>
#include <cstring>

namespace taskranger {

void TaskFilter::mutateModifyJson(nlohmann::json& inOut, const std::string key, const std::string value) {
    inOut.erase(std::remove_if(inOut.begin(), inOut.end(),
                               [key, value](const auto& json) -> bool {
                                   return json.find(key) == json.end() || json.at(key) != value;
                               }),
                inOut.end());
}

void TaskFilter::mutateModifyJson(nlohmann::json& inOut, const std::string key, const std::vector<std::string> values) {
    inOut.erase(std::remove_if(inOut.begin(), inOut.end(),
                               [key, values](const auto& json) -> bool {
                                   if (json.find(key) == json.end()) return true;
                                   auto storedValues = json.at(key);
                                   for (auto& value : values)
                                       if (std::find(storedValues.begin(), storedValues.end(), value) == storedValues.end()) return true;
                                   return false;
                               }),
                inOut.end());
}

nlohmann::json TaskFilter::filterTasks(const nlohmann::json& rawInput, std::shared_ptr<InputData> input,
                                       std::vector<std::string> dropKeys) {
    using namespace std::literals;
    auto& filters = input->tokens;
    nlohmann::json reworked;

    if (filters.find("ids") != filters.end()) {
        for (auto& id : Util::splitString(filters["ids"], ",")) {
            try {
                unsigned long long idx = std::stoull(id);
                if (idx > rawInput.size()) {
                    ColorPrinter printer;
                    printer << ANSIFeature::FOREGROUND << 9 << "Error: attempted to query ID " << idx
                            << " when there's only " << rawInput.size() << " tasks." << std::endl;
                }
                // the idx is in a standard human counting system (the first item is 1).
                // the array access index still starts at 0, so 1 needs to be subtracted
                // from the ID
                reworked.push_back(rawInput.at(idx - 1));
                reworked.back()["id"] = idx;
            } catch (std::out_of_range) {
                ColorPrinter printer;
                printer << ANSIFeature::FOREGROUND << 9 << "Error: Invalid ID: " << id << ANSIFeature::CLEAR
                        << std::endl;
            }
        }
    } else {
        // If no IDs are specified, the tasks still need some preprocessing.
        // Specifically, they need to have their IDs inserted.
        for (size_t i = 0; i < rawInput.size(); i++) {
            nlohmann::json task = rawInput.at(i);
            task["id"] = i + 1;
            reworked.push_back(task);
        }
    }

    if (input->project != "") {
        // This makes sure all projects contain an @, similarly
        // to how tags contain a +. This is intended for compatibility
        // with project:name
        if (input->project.at(0) != '@') input->project = "@"s + input->project;

        mutateModifyJson(reworked, "project", input->project);
    }

    if (input->tags.size()) {
        mutateModifyJson(reworked, "tags", input->tags);
    }

    for (auto& json : reworked) {
        for (auto& key : dropKeys) {
            json.erase(key);
        }
    }

    return reworked;
}

} // namespace taskranger
