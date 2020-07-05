#include "InfoCommand.hpp"

#include "taskranger/data/Environment.hpp"
#include "taskranger/util/TableUtil.hpp"
#include "taskranger/util/TaskFilter.hpp"

namespace taskranger {

void InfoCommand::run() {
    auto input = Environment::getInstance()->getInputData();
    auto& data = input->data;
    if (input->tags.size() == 0 && (data.size() == 0 || (data.size() == 1 && data.find("subcommand") != data.end()))) {
        // Basic guard to prevent the program from spewing out
        // lots of output
        throw "You need to specify a filter for this command";
    }
    auto& database = *Environment::getInstance()->getDatabase("active.json", true);

    tabulate::Table table;
    auto& taskVec = database.getDatabase();
    auto filteredJson = TaskFilter::Filter::createFilter(input).filterTasks(taskVec);
    if (filteredJson.size()) {
        TableUtil::TableBuilder()
                .withKeys({"Name", "Value"})
                .withUniqueTables()
                .setFilterKeys(false)
                .setTransformKeys(false)
                .build(filteredJson);
        std::cout << "\n" << filteredJson.size() << " tasks found." << std::endl;

    } else {
        throw "No tasks found";
    }
}

} // namespace taskranger
