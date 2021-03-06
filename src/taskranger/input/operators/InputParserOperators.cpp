#include "InputParserOperators.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/util/StrUtil.hpp"

namespace taskranger {

auto InputParserOperators::determineOperator(const std::string& attribKey, std::shared_ptr<Attribute>& attribPtr)
        -> std::pair<Operator, std::string> {

    auto split = StrUtil::reverseSplitString(attribKey, ".", 1);
    // To handle aliases, this needs to be the rawKey.
    // The rawKey can then be used to retrieve the actual key
    // Bonus: has to be split to deal with date weirdness
    std::string rawKey = StrUtil::splitString(split.back(), '.').front();

    attribPtr = Environment::getInstance()->getAttribute(rawKey);
    if (!attribPtr) {
        throw "Unknown attribute: " + rawKey + " (debug: .1)";
    }
    std::string key;
    if (attribPtr->getType() != FieldType::DATE) {
        key = attribPtr->getName();
    } else {
        // For dates, assume the key is the first part of the split.
        key = split.back();
    }

    // The operator
    std::string& op = split.front();

    if (op == "is" || attribKey.find('.') == std::string::npos) {
        return {Operator::IS, key};
    } else if (op == "not") {
        return {Operator::NOT, key};
    } else if (op == "greater" || op == "after") {
        return {Operator::GREATER_THAN, key};
    } else if (op == "less" || op == "before") {
        return {Operator::LESS_THAN, key};
    } else if (op == "contains") {
        return {Operator::CONTAINS, key};
    } else if (op == "greatereq") {
        return {Operator::GREATER_EQ, key};
    } else if (op == "lesseq") {
        return {Operator::LESS_EQ, key};
    }
    /**
     * If we fail to split, assume the operator is Operator::IS,
     * and that whatever was chucked in has a reason for containing
     * a `.`. This is heavily for future compatibility.
     */
    return {Operator::IS, attribKey};
}

template <>
bool InputParserOperators::evalOperator(Operator op, const std::string& input, const std::string& relativeTo) {
    switch (op) {
    case Operator::IS:
        return StrUtil::istrEquals(input, relativeTo);
    case Operator::NOT:
        return !StrUtil::istrEquals(input, relativeTo);
    case Operator::CONTAINS:
        return StrUtil::toLower(relativeTo).find(StrUtil::toLower(input)) != std::string::npos;
    default:
        return true;
    }
}

} // namespace taskranger
