#include "TaskFilter.hpp"
#include "taskranger/data/TaskInfo.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>

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
                            if (json.find(key) == json.end())
                                return true;
                            auto storedValues = json.at(key);
                            for (auto& value : values)
                                if (std::find(storedValues.begin(), storedValues.end(), value) == storedValues.end())
                                    return true;
                            return false;
                        }),
            inOut.end());
}

nlohmann::json TaskFilter::filterTasks(const nlohmann::json& rawInput, std::shared_ptr<InputData> input,
        bool includeIds, std::vector<std::string> dropKeys) {
    using namespace std::literals;
    auto filters = input->data;
    if (input->tags.size() != 0) {
        std::string builder;
        std::string subBuilder;
        for (size_t i = 0; i < input->tags.size(); i++) {
            auto tag = input->tags.at(i);
            if (tag.at(0) == '-') {
                subBuilder += (subBuilder.length() != 0 ? ","s : ""s) + "+" + tag.substr(1);
            } else {
                builder += (builder.length() != 0 ? ","s : ""s) + tag;
            }
        }
        if (builder != "") {
            filters["tags"] = builder;
        }
        if (subBuilder != "") {
            filters["tags.not"] = subBuilder;
        }
    }
    nlohmann::json reworked;

    if (filters.find("ids") != filters.end()) {
        if (!includeIds)
            return {};
        std::vector<unsigned long long> vec;

        for (auto& id : StrUtil::splitString(filters["ids"], ",")) {

            try {
                size_t endPos = 0;
                unsigned long long idx = std::stoull(id, &endPos);

                if (endPos == id.length() && idx - 1 < rawInput.size()) {
                    // Silently skip out of range IDs and incomplete conversions.
                    // The if statement is inversed to enable failed cases to skip on to UUID checks

                    // the idx is in a standard human counting system (the first
                    // item is 1). the array access index still starts at 0, so 1
                    // needs to be subtracted from the ID
                    bool skip = false;
                    for (auto& task : reworked) {
                        if (task.at("id") == idx) {
                            skip = true;
                        }
                    }
                    if (skip) {
                        continue;
                    }
                    auto cache = rawInput.at(idx - 1);
                    cache["id"] = idx;
                    reworked.push_back(cache);
                    continue;
                }
            } catch (std::invalid_argument&) {
                // Silently ignore failed IDs. Also required for UUID checking
            }
            if (id.size() > 36) {
                // Invalid IDs can be skipped. (The UUID length is 36)
                continue;
            }
            // Inefficient?
            for (size_t i = 0; i < rawInput.size(); i++) {
                auto& task = rawInput.at(i);
                auto uuid = task.at("uuid").get<std::string>();
                if (uuid == id || StrUtil::startsWith(uuid, id)) {
                    auto cache = task;
                    cache["id"] = i + 1;
                    reworked.push_back(cache);
                    break;
                }
            }
            continue;
        }
    } else {
        // If no IDs are specified, the tasks still need some preprocessing.
        // Specifically, they need to have their IDs inserted.
        for (size_t i = 0; i < rawInput.size(); i++) {
            nlohmann::json task = rawInput.at(i);
            if (includeIds)
                task["id"] = i + 1;
            else
                task["id"] = "-";
            reworked.push_back(task);
        }
    }

    // Filter the tasks
    for (auto& filter : filters) {
        auto baseKey = filter.first;
        if (baseKey == "ids" || baseKey == "subcommand")
            continue;
        auto calcPair = InputParserOperators::determineOperator(baseKey);
        InputParserOperators::Operator op = calcPair.first;
        const std::string& key = calcPair.second;
        std::string filterValue = filter.second;
        // For now: skip empty filters.
        // Will have to revisit this in the future
        if (filterValue.size() == 0)
            continue;

        if (key == "project" && filterValue.at(0) != '@')
            filterValue = "@" + filterValue;
        Task::convertAndEval(op, key, filterValue, reworked);
    }

    if (reworked.size() != 0) {
        for (auto& json : reworked) {
            for (auto& key : dropKeys) {
                json.erase(key);
            }
        }
    }

    return reworked;
}

} // namespace taskranger
