#include "UDAAttribute.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/data/attributes/AttribTypes.hpp"
#include <memory>

namespace taskranger {

std::shared_ptr<Attribute> UDAAttribute::makeAttribute(const std::string& attribute) {
    using namespace std::literals;

    auto& config = *(Environment::getInstance()->getConfig());
    auto udaListIt = config.findKey("uda");

    if (udaListIt == config.end()) {
        throw "No UDAs defined. This should never happen"s;
    }

    auto& udas = *udaListIt;
    auto udaConfIt = udas.find(attribute);
    if (udaConfIt == udas.end()) {
        throw "UDA " + attribute + " not found.";
    }

    auto& udaConf = *udaConfIt;
    // Some preprocessing later, we have the UDA.
    // If there is no type, the UDA is invalid.
    // This should probably be enforced aggressively
    auto typeIt = udaConf.find("type");
    if (typeIt == udaConf.end()) {
        throw "UDA " + attribute + " does not define a type. This is required.";
    }

    std::shared_ptr<Attribute> udaAttribPtr;

    FieldType type = typeIt->get<std::string>();
    switch (type) {
    case FieldType::STRING:
        udaAttribPtr = std::make_shared<StringAttribute>();
        break;
    case FieldType::ULLONG:
        udaAttribPtr = std::make_shared<ULLongAttribute>();
        break;
    case FieldType::NUMBER:
        udaAttribPtr = std::make_shared<NumberAttribute>();
        break;
    case FieldType::STRLIST:
        udaAttribPtr = std::make_shared<StrListAttribute>();
        break;
    case FieldType::DATE:
        udaAttribPtr = std::make_shared<DateAttribute>();
        break;
    default:
        throw "Someone fucked up and forgot to implement a branch for " + typeIt->get<std::string>();
    }

    udaAttribPtr->setUda();
    udaAttribPtr->type = type;

    auto allowedValuesIt = udaConf.find("values");
    if (allowedValuesIt != udaConf.end()) {
        auto& values = *allowedValuesIt;
        if (!values.is_array()) {
            throw "The values field for the UDA " + attribute + " is not an array";
        }
        udaAttribPtr->allowedValues = values;
    }

    // Note: value with two args sources a default argument. With the exception of the optional `values`, it rarely
    // needs to manually check
    udaAttribPtr->label = udaConf.value("label", "");
    // Link up the name
    udaAttribPtr->name = attribute;

    return udaAttribPtr;
}

} // namespace taskranger
