#pragma once

#include "taskranger/data/Task.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <variant>

namespace taskranger {
namespace TaskInfo {

template <typename T>
void insItems(std::vector<std::shared_ptr<Task>>& output, const std::vector<std::shared_ptr<Task>>& inVec,
        const std::string& fieldName, InputParserOperators::Operator op, T& input) {

    for (auto& taskObj : inVec) {
        auto& task = taskObj->getTaskJson();
        auto missesField = fieldName != "id" && task.find(fieldName) == task.end();
        // Tasks missing a field technically count towards the not operator.
        // If we're looking for tasks that have a field `fieldName` that doesn't match
        // a given `input`, tasks without the field of course match.

        if ((op == InputParserOperators::Operator::NOT && missesField) ||
                (!missesField && InputParserOperators::evalOperator(op, input, task.at(fieldName).get<T>()))) {
            output.push_back(taskObj);
        }
    }
}

void convertAndEval(InputParserOperators::Operator op, const std::string& fieldName, const std::string& rawInput,
        std::vector<std::shared_ptr<Task>>& output, std::vector<std::shared_ptr<Task>>& input);

/**
 * Overload for timestamps
 *
 * @param operator      The operator to use
 * @param fieldName     The attribute name
 * @param tp            The timepoint supplied as milliseconds for operator reasons
 */
void convertAndEval(InputParserOperators::Operator op, const std::string& fieldName, int64_t tp,
        std::vector<std::shared_ptr<Task>>& output, std::vector<std::shared_ptr<Task>>& input);

} // namespace TaskInfo
} // namespace taskranger
