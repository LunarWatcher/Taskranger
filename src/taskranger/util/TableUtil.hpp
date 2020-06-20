#ifndef TASKRANGER_UTIL_TABLEUTIL_HPP
#define TASKRANGER_UTIL_TABLEUTIL_HPP

#include "tabulate/table.hpp"
#include "taskranger/data/Task.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <variant>
#include <vector>

namespace taskranger {

namespace TableUtil {

typedef std::variant<std::string, tabulate::Table> TableRow;

/**
 * Converts the default JSON data to
 *
 * @param renderTarget      A JSON array containing JSON objects to render to a table
 */
tabulate::Table renderTasks(
        std::vector<std::shared_ptr<Task>>& renderTarget, std::map<std::string, int> keyPriority = {});

} // namespace TableUtil

} // namespace taskranger

#endif
