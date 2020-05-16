#ifndef TASKRANGER_UTIL_TABLEUTIL_HPP
#define TASKRANGER_UTIL_TABLEUTIL_HPP

#include "tabulate/table.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <variant>

namespace taskranger {

namespace TableUtil {

typedef std::variant<std::string, tabulate::Table> TableRow;

/**
 * Converts the default JSON data to
 *
 * @param renderTarget      A JSON array containing JSON objects to render to a table
 */
tabulate::Table renderTasks(nlohmann::json& renderTarget, std::map<std::string, int> keyPriority = {});

}

}

#endif
