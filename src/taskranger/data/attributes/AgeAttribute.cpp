#include "AgeAttribute.hpp"

#include "taskranger/util/DatetimeUtil.hpp"

namespace taskranger {

Types::TableRow AgeAttribute::getMinimalRepresentationForTable(const nlohmann::json& json) {
    auto it = json.find(this->name);
    if (it == json.end()) {
        return " ";
    }

    return DateTimeUtil::formatRelative(it->get<double>());
}

} // namespace taskranger
