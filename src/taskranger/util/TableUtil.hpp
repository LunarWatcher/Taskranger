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

class TableBuilder {
private:
    std::map<std::string, std::vector<TableRow>> columns;
    std::vector<std::string> keys;

public:
    TableBuilder();

    /**
     * Determines which keys to display, as well as the order.
     */
    TableBuilder& withKeys(const std::vector<std::string>& keys);

    /**
     * Builds the table
     */
    tabulate::Table build(std::vector<std::shared_ptr<Task>>& tasks);
};

} // namespace TableUtil

} // namespace taskranger

#endif
