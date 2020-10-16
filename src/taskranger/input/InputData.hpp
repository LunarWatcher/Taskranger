#ifndef TASKRANGER_INPUT_INPUTDATA_HPP
#define TASKRANGER_INPUT_INPUTDATA_HPP

#include "taskranger/util/StrUtil.hpp"
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace taskranger {

class InputData {
public:
    /**
     * Contains general tokens that can be represented as a string/string
     * key-value pair. Other types of data are stored in other ways.
     *
     * Depending on the command context, this may also contain filters.
     * // <key, <attributes<rest of the command, input>>
     */
    std::map<std::string, std::string> data;
    std::vector<std::string> tags;

    void removeTokens(const std::vector<std::string>& invalidFields);
    void removeInternalFields();

    void createSelectiveFrom(std::shared_ptr<InputData> orig, std::vector<std::string> legalKeys);

    std::map<std::string, std::string> getData(std::vector<std::string> blacklistedKeys) {
        std::map<std::string, std::string> data = this->data;
        bool tags = true;
        for (auto& k : blacklistedKeys) {
            auto it = data.find(k);
            if (it != data.end()) {
                data.erase(it);
            } else if (k == "tags") {
                tags = false;
            }
        }
        if (tags) {
            data["tags"] = std::accumulate(this->tags.begin(), this->tags.end(), std::string(""),
                    [](std::string a, std::string b) { return (a.size() == 0 ? "" : a + ",") + b; });
        }
        return data;
    }
};

} // namespace taskranger

#endif
