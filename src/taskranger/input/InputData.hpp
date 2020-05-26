#ifndef TASKRANGER_INPUT_INPUTDATA_HPP
#define TASKRANGER_INPUT_INPUTDATA_HPP

#include <map>
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
     */
    std::map<std::string, std::string> tokens;

    /**
     * Vector of the tags. Can be empty if no tags are supplied, or
     * they're not relevant for the context.
     */
    std::vector<std::string> tags;

    // The project, group, category, or whatever other term works.
    std::string project;

    void removeTokens(const std::vector<std::string>& invalidFields);
    void removeInternalFields();
};

} // namespace taskranger

#endif
