#pragma once

#include "taskranger/input/operators/InputParserOperators.hpp"
#include <functional>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <variant>

namespace taskranger {
namespace Task {

/**
 * Default removeIf predicate for tasks.
 *
 * Returns true if the task should be removed, and false otherwise.
 */
template <typename T>
void insItems(nlohmann::json& output, const nlohmann::json& inVec, const std::string& fieldName,
        InputParserOperators::Operator op, T& input) {

    for (auto& task : inVec) {

        auto missesField = task.find(fieldName) == task.end();
        // Tasks missing a field technically count towards the not operator.
        // If we're looking for tasks that have a field `fieldName` that doesn't match
        // a given `input`, tasks without the field of course match.
        // in the event the evalOperator output determines whether the task is removed or not,
        // this function has to return the _opposite_ of what the evalOperator function returns.
        // `false` from evalOperator means the predicate doesn't match, and it should be removed.
        // `true` from evalOperator means the predicate does match, and it shouldn't be removed.
        if ((op == InputParserOperators::Operator::NOT && missesField) ||
                (!missesField && InputParserOperators::evalOperator(op, input, task.at(fieldName).get<T>()))) {
            output.push_back(task);
        }
    }
}

void convertAndEval(InputParserOperators::Operator op, const std::string& fieldName, const std::string& rawInput,
        nlohmann::json& reworked, const nlohmann::json& orig);

} // namespace Task
} // namespace taskranger
