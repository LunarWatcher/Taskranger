#include "NextCommand.hpp"
#include <iostream>
#include "tabulate/table.hpp"

namespace taskranger {

void NextCommand::run(std::shared_ptr<InputData> input) {
    tabulate::Table taskTable;
    taskTable.add_row({"ID", "Description", "Tags", "Category"});
    taskTable.add_row({"0", "Get cake or something lmao", "+Personal", "Yayeet"});
    taskTable.add_row({"1", "This is an example btw", "+DummyData", "I don't use arch btw"});
    taskTable.format()
        .border_color(tabulate::Color::yellow);
    std::cout << taskTable << std::endl;
}

}
