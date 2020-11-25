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

/**
 * Utility for checking if a field matches the conditions supplied.
 */
template <typename T>
bool checkTask(InputParserOperators::Operator op, const std::string& fieldName, const nlohmann::json& taskJson,
        const T& input) {
    // TODO: rewrite this steaming pile of garbage

    // Tasks missing a field technically count towards the not operator.
    // If we're looking for tasks that have a field `fieldName` that doesn't match
    // a given `input`, tasks without the field of course match.
    auto attempt = taskJson.find(fieldName);
    // Now, this variable is initially what kept a bug at bay. However, the intro
    // of vtags made shit substantially more difficult. Merging in a different attribute
    // while keeping the same inclusion type required additional logic.
    // Not sure if I can make tags disjoint or not, but that won't be easy either.
    // Admittedly, this approach is already cleaner than the one I had a few minutes
    // ago that involved 3 bools doing the same thing, lul
    auto missesField = attempt == taskJson.end();

    // So if we can't find it...
    if (attempt == taskJson.end()) {
        // and we're a string vector...
        if constexpr (std::is_same<T, std::vector<std::string>>()) {
            // check if this is tags
            if (fieldName == "tags") {
                // and insert the attempt
                attempt = taskJson.find("vtags");
                if (attempt != taskJson.end()) {
                    // If all is good, we can say we're not missing the field, even
                    // if we technically are
                    missesField = false;
                }
            }
        }
        // otherwise, do nothing, missesField is still true, so...
    }

    auto fieldValue = missesField ? T{} // This one is safely ignored, and thanks to default constructors basically
                                        // doing nothing (I hope :blobsweat:), it shouldn't affect complexity too much.
                                  : attempt->get<T>(); // Otherwise, we have an easy situation in which
                                                       // we just get the shit we found.
    // Also note that any initialization in general is required now that getting the field tries to happen
    // regardless of its existence.
    // The result is ignored thanks to missesField, so the consequences shouldn't be too big.

    if constexpr (std::is_same<T, std::vector<std::string>>()) {
        if (fieldName == "tags") {
            // on the other hand, if the field _isn't_ missing, we still need to inject the vtags.
            // This split solution is why I want to rewrite this trash, but that's a job for
            // future me. (Suuuckkkerrr :P -- past you)
            if (!missesField) {
                auto vTagIt = taskJson.find("vtags");
                if (vTagIt != taskJson.end()) {
                    auto vTags = vTagIt->get<T>();
                    fieldValue.insert(fieldValue.end(), vTags.begin(), vTags.end());
                }
            }
        }
    }

    return (op == InputParserOperators::Operator::NOT && missesField) ||
           (!missesField && InputParserOperators::evalOperator(op, input, fieldValue));
}

} // namespace TaskFilter

} // namespace taskranger

#endif
