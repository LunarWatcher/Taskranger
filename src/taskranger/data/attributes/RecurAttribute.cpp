#include "RecurAttribute.hpp"

namespace taskranger {

void RecurAttribute::modify(nlohmann::json& task, const std::string& key, const std::string& input) {}

Types::TableRow RecurAttribute::getMinimalRepresentationForTable(const nlohmann::json& task) {}

Types::TableRow RecurAttribute::getMaxRepresentationForTable(const nlohmann::json& task) {}

} // namespace taskranger
