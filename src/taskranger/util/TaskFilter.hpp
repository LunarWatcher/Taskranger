#ifndef TASKRANGER_UTIL_TASKFILTER_HPP
#define TASKRANGER_UTIL_TASKFILTER_HPP

#include "nlohmann/json.hpp"
#include "taskranger/data/Task.hpp"
#include "taskranger/input/InputData.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include <map>
#include <memory>
#include <vector>

namespace taskranger {

namespace TaskFilter {

// TOOO include operator modification (i.e. for dates and numbers; AKA
// extend beyond strings)
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
 * Filters JSON input by command-supplied filters, and optionally by
 * user-supplied filters
 */
std::vector<std::shared_ptr<Task>> filterTasks(
        const std::vector<std::shared_ptr<Task>>& rawInput, std::shared_ptr<InputData> input);

} // namespace TaskFilter

} // namespace taskranger

#endif
