#include "AllCommand.hpp"
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

void AllCommand::run(std::shared_ptr<InputData> input) {
    JSONDatabase active("active.json");
    JSONDatabase historical("completed.json");

    auto filtered = TaskFilter::filterTasks(*active.getDatabase(), input, true);
    auto hFiltered = TaskFilter::filterTasks(*historical.getDatabase(), input, false);
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
