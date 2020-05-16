#include "NextCommand.hpp"
#include "tabulate/table.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/TableUtil.hpp"
#include "taskranger/util/TaskFilter.hpp"
#include <cstdint>
#include <iostream>
#include <map>

namespace taskranger {

void NextCommand::run(std::shared_ptr<InputData> input) {
    typedef std::variant<std::string, tabulate::Table> GarbageType;
    JSONDatabase database("active.json");

    if (database.getDatabase()->size() == 0) {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9 << "No tasks found." << ANSIFeature::CLEAR << std::endl;
        return;
    }

    tabulate::Table table;
    auto& taskVec = *database.getDatabase();
    auto filteredJson = TaskFilter::filterTasks(taskVec, input, { "uuid" });
    std::cout << TableUtil::renderTasks(filteredJson, {{"id", 1}, {"description", 2}})<< std::endl;;
}

} // namespace taskranger
