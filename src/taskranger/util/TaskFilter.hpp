#ifndef TASKRANGER_UTIL_TASKFILTER_HPP
#define TASKRANGER_UTIL_TASKFILTER_HPP

#include "nlohmann/json.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/Task.hpp"
#include "taskranger/input/InputData.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include <map>
#include <memory>
#include <vector>

namespace taskranger {

namespace TaskFilter {

struct FilterInfo {
    InputParserOperators::Operator op;
    FieldType fieldType;
    std::vector<std::any> inputs;
    std::string fieldName;
};

class Filter {
private:
    std::vector<std::shared_ptr<FilterInfo>> filters;

public:
    std::vector<std::shared_ptr<Task>> filterTasks(const std::vector<std::shared_ptr<Task>>& rawInput);
    Filter& disableConditionally(const std::string& field, const std::string& value);

    static Filter createFilter(std::shared_ptr<InputData> input);
};

/**
 * Modifies the JSON for string key string value pairs. Drops task entries
 * that don't have a field containing the given key, or that has a different
 * value than the provided value.
 *
 * @param inOut         The JSON input and output
 * @param key           The key in the JSON to check
 * @param value         The expected value of the field.
 */
void mutateModifyJson(nlohmann::json& inOut, const std::string key, const std::string value);
void mutateModifyJson(nlohmann::json& inOut, const std::string key, const std::vector<std::string> values);

/**
 * Utility for checking if a field matches the conditions supplied.
 */
template <typename T>
bool checkTask(
        InputParserOperators::Operator op, const std::string& fieldName, const nlohmann::json& taskJson, T& input) {
    auto missesField = taskJson.find(fieldName) == taskJson.end();
    // Tasks missing a field technically count towards the not operator.
    // If we're looking for tasks that have a field `fieldName` that doesn't match
    // a given `input`, tasks without the field of course match.

    return (op == InputParserOperators::Operator::NOT && missesField) ||
           (!missesField && InputParserOperators::evalOperator(op, input, taskJson.at(fieldName).get<T>()));
}

} // namespace TaskFilter

} // namespace taskranger

#endif
