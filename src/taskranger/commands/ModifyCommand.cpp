#include "ModifyCommand.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/util/ConfirmationHelper.hpp"
#include "taskranger/util/TaskFilter.hpp"

namespace taskranger {

void ModifyCommand::run() {
    auto environment = Environment::getInstance();
    auto input = environment->getInputData();
    if (input->data.find("subcommand") != input->data.end()) {
        input->data.erase("subcommand");
    }

    auto active = environment->getDatabase("active.json", true);
    auto completed = environment->getDatabase("completed.json", false);

    // To limit the filter, we need to drop all the fields except
    // the ID fields and their aliases, as well as the UUID
    auto filterData = std::make_shared<InputData>();
    filterData->createSelectiveFrom(input, {"id", "ids", "uuid"});

    auto filter = TaskFilter::Filter::createFilter(filterData);
    auto aFiltered = filter.filterTasks(active->getDatabase());
    auto cFiltered = filter.filterTasks(completed->getDatabase());

    aFiltered.insert(aFiltered.end(), cFiltered.begin(), cFiltered.end());

    auto config = environment->getConfig();

    std::map<std::string, std::string> attribData = input->getData({"id", "ids", "uuid"});
    ConfirmationHelper helper(attribData, aFiltered);
    size_t processed = helper.process();
    if (processed != 0) {
        active->commit();
        completed->commit();
        std::cout << processed << " tasks modified.\n";
    } else {
        std::cout << "No tasks modified.\n";
    }
}

} // namespace taskranger
