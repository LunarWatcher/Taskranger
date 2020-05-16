#ifndef TASKRANGER_UTIL_TASKFILTER_HPP
#define TASKRANGER_UTIL_TASKFILTER_HPP

#include "nlohmann/json.hpp"
#include "taskranger/input/InputData.hpp"
#include <map>
#include <memory>
#include <vector>

namespace taskranger {

namespace TaskFilter {
    /**
     * Filters JSON input by command-supplied filters, and optionally by user-supplied filters
     */
    nlohmann::json filterTasks(const nlohmann::json& rawInput, std::shared_ptr<InputData> input,
                                            std::vector<std::string> dropKeys = {});
}

} // namespace taskranger

#endif
