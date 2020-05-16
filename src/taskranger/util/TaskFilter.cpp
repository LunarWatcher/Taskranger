#include "TaskFilter.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <cstring>

namespace taskranger {

nlohmann::json TaskFilter::filterTasks(const nlohmann::json& rawInput,
        std::shared_ptr<InputData> input, std::vector<std::string> dropKeys) {
    auto& filters = input->tokens;
    nlohmann::json reworked;

    if (filters.find("ids") != filters.end()) {
        for (auto& id : Util::splitString(filters["ids"], ",")) {
            try {
                unsigned long long idx = std::stoull(id);
                if (idx > rawInput.size()) {
                    ColorPrinter printer;
                    printer << ANSIFeature::FOREGROUND
                        << 9
                        << "Error: attempted to query ID "
                        << idx
                        << " when there's only "
                        << rawInput.size()
                        << " tasks." << std::endl;
                }
                reworked.push_back(rawInput.at(idx));
            } catch(std::out_of_range) {
                ColorPrinter printer;
                printer << ANSIFeature::FOREGROUND << 9
                    << "Error: Invalid ID: " << id << ANSIFeature::CLEAR << std::endl;
            }
        }
    } else {
        for (size_t i = 0; i < rawInput.size(); i++) {
            nlohmann::json task = rawInput.at(i);
            task["id"] = i + 1;
            reworked.push_back(task);
        }
    }

    for (auto& json : reworked) {
        for (auto& key : dropKeys) {
            json.erase(key);
        }
    }

    return reworked;
}

}
