#include "DoneCommand.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <functional>
#include <set>
#include <stdexcept>

namespace taskranger {

DoneCommand::DoneCommand() {
    this->name = "done";
    this->help = "Marks one or more tasks as done";
    this->help = "taskranger done <ids>";
}

void DoneCommand::run() {
    auto input = Environment::getInstance()->getInputData();
    auto& data = input->data;

    std::vector<unsigned long long> ids;

    if (data.find("description") != data.end()) {
        // The description ones are space separated
        auto strIds = StrUtil::splitString(data.at("description"), " ");
        if (strIds.size() > 0)
            StrUtil::strVecToUll(strIds, ids);
    }
    if (data.find("ids") != data.end()) {
        auto strIds = StrUtil::splitString(data.at("ids"), ",");
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
    std::set<unsigned long long> mSet;
    for (auto& id : ids)
        mSet.insert(id);
    ids.assign(mSet.begin(), mSet.end());

    for (auto revIt = ids.rbegin(); revIt != ids.rend(); revIt++) {

        auto id = *revIt;

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
