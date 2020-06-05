#include "Attribute.hpp"
#include "taskranger/data/Environment.hpp"

#include "taskranger/data/attributes/AttribTypes.hpp"
#include "taskranger/data/attributes/ProjectAttribute.hpp"
#include "taskranger/data/attributes/TagsAttribute.hpp"
#include "taskranger/data/attributes/UUIDAttribute.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>

namespace taskranger {

bool Attribute::isValueAllowed(const std::string& input) {
    // If there's no restrictions, it's fine.
    if (!allowedValues.has_value())
        return true;
    // Otherwise...
    auto& allVals = allowedValues.value();
    return std::find(allVals.begin(), allVals.end(), input) != allVals.end();
}

std::shared_ptr<Attribute> Attribute::createAttrib(const std::string& fieldName) {
    using namespace std::literals;
    // Shorthand for Make Shared Attribute
    // Mainly because typing std::make_shared<Attribute> over and over
    // fucking sucks.

    if (fieldName == "id") {
        return std::make_shared<IDAttribute>();
    } else if (fieldName == "description") {
        return std::make_shared<StringAttribute>();
    } else if (fieldName == "tags") {
        return std::make_shared<TagsAttribute>();
    } else if (fieldName == "project") {
        return std::make_shared<ProjectAttribute>();
    } else if (fieldName == "uuid") {
        return std::make_shared<UUIDAttribute>();
    } else {
        auto& config = *Environment::getInstance()->getConfig();
        auto udaIt = config.findKey("uda");
        if (udaIt == config.end()) {
            throw "Unknown attribute: "s + fieldName;
        }
        nlohmann::json& udas = *udaIt;
        auto attribIt = udas.find(fieldName);
        if (attribIt == udas.end()) {
            throw "Unknown attribute: "s + fieldName;
        }

        if (attribIt->find("type") == attribIt->end()) {
            throw "Misconfigured attribute: no type for UDA "s + fieldName;
        }
        throw "I haven't been implemented yet."s;
    }
    return nullptr;
}

} // namespace taskranger
