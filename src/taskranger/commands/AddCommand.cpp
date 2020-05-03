#include "AddCommand.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "tabulate/table.hpp"
namespace taskranger {

void AddCommand::run(std::shared_ptr<InputData> input) {
    auto& tokens = input->tokens;
    if (tokens.find("content") == tokens.end()) {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9
            << "You need to add a message to the todo"
            << std::endl;
        return;
    }

    JSONDatabase database("active.json");
    (*database.getDatabase()).push_back(tokens);
    tabulate::Table table;
    for (auto& item : *database.getDatabase()) {
        std::vector<std::string> values;
        for (const auto& sItem : item ) {
            if (sItem.is_string())
                values.push_back(sItem);
            else if(sItem.is_number())
                values.push_back(std::to_string(sItem.get<long long>()));
            else
                std::cout << "Fix your shit, Olivia" << std::endl;
        }
    }
    database.commit();
}

}
