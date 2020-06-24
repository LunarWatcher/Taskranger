#ifndef TASKRANGER_INPUT_INPUTDATA_HPP
#define TASKRANGER_INPUT_INPUTDATA_HPP

#include "taskranger/util/StrUtil.hpp"
#include <map>
#include <memory>
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
};

} // namespace taskranger

#endif
