#pragma once

#include "taskranger/util/StrUtil.hpp"
#include <any>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace taskranger {

// clang-format off
enum class FieldType {
    STRING,
    /**
     * Unsigned long long
     */
    ULLONG,
    DATE,
    /**
     * List of strings
     */
    STRLIST
};
// clang-format on

inline FieldType strToFT(const std::string& fieldType) {
    if (StrUtil::istrEquals(fieldType, "string")) {
        return FieldType::STRING;
    } else if (StrUtil::istrEquals(fieldType, "ullong")) {
        return FieldType::ULLONG;
    } else if (StrUtil::istrEquals(fieldType, "strlist")) {
        return FieldType::STRLIST;
    } else if (StrUtil::istrEquals(fieldType, "date")) {
        return FieldType::DATE;
    }
    throw "Unknown field type: " + fieldType;
}

class Attribute {
protected:
    /**
     * The attribute name.
     */
    std::string name;
    /**
     * The attribute label. This is show in tables over
     * the name
     *
     * Optional (empty => use name)
     */
    std::string label;

    /**
     * The type of value this attribute supports.
     */
    FieldType type;

    /**
     * Whether or not the attribute is built-in or not.
     *
     * False for user-defined attribs
     */
    bool builtin = true;

    /**
     * Contains allowed values, if there are any.
     * This is mainly for user defined attribs.
     *
     * Note that the value is stored as a string, but all types
     * of values are allowed. Storing as a string is a design
     * choice to save some minor amount of time and resources
     * by not having to use std::any, and/or std::variant, and get
     * stuck with a type conversion clusterfuck.
     */
    std::optional<std::vector<std::string>> allowedValues;

public:
    Attribute() {}
    virtual ~Attribute() = default;

    virtual void modify(nlohmann::json& /* task */, const std::string& /* input */){};

    /**
     * This function is meant to validate the value of the field.
     * This is implemented in child classes.
     *
     * Fields may also modify the attribValue if necessary.
     */
    virtual void validate(nlohmann::json& /* attribValue */) {}

    const std::string& getName() {
        return name;
    }

    /**
     * Returns whether a given value is allowed for the field or not.
     *
     * Always returns true if there's no value limitation.
     * Otherwise, returns whether the value is contained in the set
     * of @{allowedValues}
     */
    bool isValueAllowed(const std::string& input);

    bool isBuiltin() {
        return builtin;
    }

    bool hasLimitedValues() {
        return allowedValues.has_value();
    }

    static std::shared_ptr<Attribute> createAttrib(const std::string& fieldName);
};

} // namespace taskranger
