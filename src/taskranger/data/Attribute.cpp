#include "Attribute.hpp"
#include "taskranger/data/Environment.hpp"

// I personally don't like this type of unnecessary subclass use, but there's
// not a lot of other alternatives.
// The other alternative is using typed subclasses where modify() isn't
// explicitly needed as an override. Unfortunately, this still requires manually
// settings vars, and because there's no decent hacks to make it look pretty,
// it just looks a lot ugler than subclassing
#include "taskranger/data/attributes/AttribTypes.hpp"
#include "taskranger/data/attributes/DescriptionAttribute.hpp"
#include "taskranger/data/attributes/DueAttribute.hpp"
#include "taskranger/data/attributes/ProjectAttribute.hpp"
#include "taskranger/data/attributes/TagsAttribute.hpp"
#include "taskranger/data/attributes/UDAAttribute.hpp"
#include "taskranger/data/attributes/UUIDAttribute.hpp"
#include "taskranger/util/StrUtil.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>

namespace taskranger {

bool Attribute::isValueAllowed(const nlohmann::json& input) {
    // If there's no restrictions, it's fine.
    if (!allowedValues.has_value()) {
        return true;
    } else if (!allowedValues->is_array()) {
        throw this->name + " specifices allowed values, but does not use an array.";
    }
    // Otherwise...
    auto& allVals = allowedValues.value();
    return std::find(allVals.begin(), allVals.end(), input) != allVals.end();
}

void Attribute::checkField(nlohmann::json& attributeValue) {
    validate(attributeValue);
    if (!isValueAllowed(attributeValue)) {
        throw "Invalid value: " + attributeValue.dump() + ". Value may only be " + this->allowedValues->dump();
    }
}

std::shared_ptr<Attribute> Attribute::createAttrib(const std::string& fieldName) {
    using namespace std::literals;

    if (fieldName == "id" || fieldName == "ids") {
        return std::make_shared<IDAttribute>();
    } else if (fieldName == "description") {
        return std::make_shared<DescriptionAttribute>();
    } else if (fieldName == "tags") {
        return std::make_shared<TagsAttribute>();
    } else if (fieldName == "project") {
        return std::make_shared<ProjectAttribute>();
    } else if (fieldName == "uuid") {
        return std::make_shared<UUIDAttribute>();
    } else if (fieldName == "due") {
        return std::make_shared<DueAttribute>();
    } else {
        auto& config = *Environment::getInstance()->getConfig();
        auto udaIt = config.findKey("uda");

        if (udaIt == config.end()) {
            throw "Unknown attribute: " + fieldName;
        }

        nlohmann::json& udas = *udaIt;
        auto attribIt = udas.find(fieldName);
        if (attribIt == udas.end()) {
            throw "Unknown attribute: " + fieldName;
        }

        if (attribIt->find("type") == attribIt->end()) {
            throw "Misconfigured attribute: no type for UDA " + fieldName;
        }

        return UDAAttribute::makeAttribute(fieldName);
    }
    return nullptr;
}

} // namespace taskranger
