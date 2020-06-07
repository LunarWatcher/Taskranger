#include "NextCommand.hpp"
#include "tabulate/table.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/TableUtil.hpp"
#include "taskranger/util/TaskFilter.hpp"
#include <cstdint>
#include <iostream>
#include <map>

namespace taskranger {

NextCommand::NextCommand() {
    this->name = "next";
    this->help = "Shows the currently active tasks.";
    this->usage = "taskranger next <filter>";
}

void NextCommand::run() {
    auto input = Environment::getInstance()->getInputData();
    JSONDatabase database("active.json");

    if (database.getDatabase()->size() == 0) {
        ColorPrinter printer;
        // clang-format off
        printer
            << ANSIFeature::FOREGROUND << 9
            << "No tasks found."
            << ANSIFeature::CLEAR
            << "\n";
        // clang-format on
        return;
    }

    tabulate::Table table;
    auto& taskVec = *database.getDatabase();
    auto filteredJson = TaskFilter::filterTasks(taskVec, input, true, {"uuid"});
    if (filteredJson.size())
        std::cout << TableUtil::renderTasks(filteredJson, {{"id", 1}, {"description", 2}}) << "\n\n";
    // clang-format off
    std::cout
        << filteredJson.size() << " tasks found."
        << "\n";
    // clang-format on
}

} // namespace taskranger
