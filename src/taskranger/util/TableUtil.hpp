#ifndef TASKRANGER_UTIL_TABLEUTIL_HPP
#define TASKRANGER_UTIL_TABLEUTIL_HPP

#include "tabulate/table.hpp"
#include "taskranger/data/Task.hpp"
#include "taskranger/metadata/Types.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <variant>
#include <vector>

namespace taskranger {

namespace TableUtil {

class TableBuilder {
private:
    std::map<std::string, std::vector<Types::TableRow>> columns;
    std::vector<std::string> keys;

    bool uniqueTables{false};
    bool filterKeys{true};
    bool transformKeys{true};

    /**
     * Adds a background to every other row of the supplied table.
     * Row 0 excluded
     */
    void fixBackground(tabulate::Table& table);

public:
    TableBuilder();

    /**
     * Determines which keys to display, as well as the order.
     */
    TableBuilder& withKeys(const std::vector<std::string>& keys);

    /**
     * Builds one table per task, using vertical rows instead of horizontal
     * Taking InfoCommand as an example:
     *
     * Name             Value
     * description      Some task
     * created          04.07.2020 00:00:00 (22 hours ago)
     * ...
     */
    TableBuilder& withUniqueTables() {
        this->uniqueTables = true;
        return *this;
    }

    TableBuilder& setFilterKeys(bool newValue) {
        this->filterKeys = newValue;
        return *this;
    }

    TableBuilder& setTransformKeys(bool newValue) {
        this->transformKeys = newValue;
        return *this;
    }

    /**
     * Builds the table
     */
    void build(std::vector<std::shared_ptr<Task>>& tasks);
};

} // namespace TableUtil

} // namespace taskranger

#endif
