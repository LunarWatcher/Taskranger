#include "DoneCommand.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <functional>
#include <stdexcept>

namespace taskranger {

DoneCommand::DoneCommand() {
    this->name = "done";
    this->help = "Marks one or more tasks as done";
    this->help = "taskranger done <ids>";
}

void DoneCommand::run(std::shared_ptr<InputData> input) {
    auto& tokens = input->tokens;

    std::vector<unsigned long long> ids;

    if (tokens.find("description") != tokens.end()) {
        // The description ones are space separated
        auto strIds = StrUtil::splitString(tokens.at("description"), " ");
        if (strIds.size() > 0)
            StrUtil::strVecToUll(strIds, ids);
    }
    if (tokens.find("ids") != tokens.end()) {
        auto strIds = StrUtil::splitString(tokens.at("ids"), ",");
        if (strIds.size() > 0)
            StrUtil::strVecToUll(strIds, ids);
    }

    if (ids.size() == 0) {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9 << "Specify some task IDs to complete." << ANSIFeature::CLEAR << "\n";
        return;
    }
    JSONDatabase active("active.json");
    JSONDatabase completed("completed.json");

    auto currentSetPtr = active.getDatabase();
    auto completedSetPtr = completed.getDatabase();
    unsigned int complete = 0;
    std::sort(ids.begin(), ids.end(), std::less<unsigned long long>());
    // unsigned types do not play well with reverse for loops
    for (long long i = ids.size() - 1; i >= 0; i--) {

        auto id = ids.at(i);

        if (id == 0)
            continue;
        if (id > currentSetPtr->size()) {
            std::cout << "Skipped task " << id << " - invalid ID" << std::endl;
            continue;
        }
        // TODO: strip now invalid properties from it
        completedSetPtr->push_back(currentSetPtr->at(id - 1));
        currentSetPtr->erase(id - 1);
        complete++;
    }

    active.commit();
    completed.commit();
    ColorPrinter printer;
    printer << ANSIFeature::FOREGROUND << 10 << "Marked " << complete << " task" << (complete == 1 ? "" : "s")
            << " as completed." << ANSIFeature::CLEAR << std::endl;
}

} // namespace taskranger
