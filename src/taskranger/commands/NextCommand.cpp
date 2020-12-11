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
    auto& database = *Environment::getInstance()->getDatabase("active.json", true);

    if (database.size() == 0) {
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
    auto& taskVec = database.getDatabase();
    auto filteredJson = TaskFilter::Filter::createFilter(input) //
                                .disableConditionally("tags", "+BLOCKED")
                                .disableConditionally("tasktype", "rtemplate", FieldType::STRING)
                                .filterTasks(taskVec);
    if (filteredJson.size())
        TableUtil::TableBuilder()
                .withKeys({"id", "description", "recur", "tags", "project", "due", "created"})
                .build(filteredJson);

    // clang-format off
    std::cout
        << "\n"
        << filteredJson.size() << " tasks found."
        << "\n";
    // clang-format on
}

} // namespace taskranger
