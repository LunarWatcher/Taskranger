#include "AgeAttribute.hpp"

#include "taskranger/util/DatetimeUtil.hpp"

namespace taskranger {

Types::TableRow AgeAttribute::getMinimalRepresentationForTable(const Task& task) {
    auto& json = task.getTaskJson();
    auto it = json.find(this->name);
    if (it == json.end()) {
        return " ";
    }

    return DateTimeUtil::formatRelative(it->get<double>());
}

} // namespace taskranger
