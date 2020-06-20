#include "AllCommand.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/input/InputData.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/TableUtil.hpp"
#include "taskranger/util/TaskFilter.hpp"
#include <cstdlib>

namespace taskranger {

AllCommand::AllCommand() {
    this->name = "all";
    this->help = "Shows all the tasks, including completed ones. "
                 "For obvious reasons, this won't be able to show deleted tasks.";
    this->usage = "taskranger all <filters>";
}

void AllCommand::run() {
    auto input = Environment::getInstance()->getInputData();
    auto& active = *Environment::getInstance()->getDatabase("active.json", true);
    auto& historical = *Environment::getInstance()->getDatabase("completed.json", false);

    auto filtered = TaskFilter::filterTasks(active.getDatabase(), input);
    auto hFiltered = TaskFilter::filterTasks(historical.getDatabase(), input);
    if (hFiltered.size() != 0) {
        if (filtered.size() != 0)
            filtered.insert(filtered.end(), hFiltered.begin(), hFiltered.end());
        else
            filtered = hFiltered;
    }
    if (filtered.size() == 0) {

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

    std::cout << TableUtil::renderTasks(filtered, {{"id", 1}, {"description", 2}}) << "\n\n";
    std::cout << filtered.size() << " tasks found.\n";
}

} // namespace taskranger
