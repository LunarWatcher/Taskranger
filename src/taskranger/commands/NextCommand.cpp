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
    JSONDatabase database("active.json");

    if (database.getDatabase()->size() == 0) {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9 << "No tasks found." << ANSIFeature::CLEAR << std::endl;
        return;
    }

    tabulate::Table table;
    auto& taskVec = *database.getDatabase();
    auto filteredJson = TaskFilter::filterTasks(taskVec, input, { "uuid" });
    if (filteredJson.size())
        std::cout << TableUtil::renderTasks(filteredJson, {{"id", 1}, {"description", 2}}) << "\n\n";
    // clang-format off
    std::cout
        << filteredJson.size() << " tasks found."
        << std::endl;
    // clang-format on
}

} // namespace taskranger
