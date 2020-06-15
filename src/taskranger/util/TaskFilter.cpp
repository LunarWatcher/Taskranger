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
    if (includeIds) {
        // If the task needs to include IDs, insert them now.
        for (size_t i = 0; i < rawInput.size(); i++) {
            nlohmann::json task = rawInput.at(i);
            if (includeIds) {
                task["id"] = i + 1;
            } else {
                task["id"] = "-";
            }
            reworked.push_back(task);
        }
    }
    nlohmann::json output;
    // Filter the tasks
    for (auto& filter : filters) {
        auto baseKey = filter.first;
        if (baseKey == "subcommand") {
            continue;
        }

        auto calcPair = InputParserOperators::determineOperator(baseKey);
        InputParserOperators::Operator op = calcPair.first;
        const std::string& key = calcPair.second;
        std::string filterValue = filter.second;

        // For now: skip empty filters.
        // Will have to revisit this in the future
        if (filterValue.size() == 0) {
            continue;
        }
        if (key == "id" && !includeIds) {
            // If we're querying IDs, but don't want to include IDs, this should
            // return an empty value
            return {};
        }

        if (key == "project" && filterValue.at(0) != '@') {
            filterValue = "@" + filterValue;
        }

        for (auto& value : StrUtil::splitString(filterValue, ',')) {
            Task::convertAndEval(op, key, value, output, reworked);
        }
        reworked = output;
        output.clear();
    }

    if (reworked.size() != 0) {
        for (auto& json : reworked) {
            for (auto& key : dropKeys) {
                if (json.find(key) != json.end())
                    json.erase(key);
            }
        }
    }

    return reworked;
}

} // namespace taskranger
