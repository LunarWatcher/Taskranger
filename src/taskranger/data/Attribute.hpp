#pragma once

#include "taskranger/util/StrUtil.hpp"
#include <any>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace taskranger {

class Attribute;

namespace UDAAttribute {
std::shared_ptr<Attribute> makeAttribute(const std::string&);
}

class FieldType {
public:
    // clang-format off
    enum FieldTypeVals {
        STRING,
        NUMBER,
        /**
         * Unsigned long long.
         * User-defined attributes may not use this field type
         */
        ULLONG,
        DATE,
        /**
         * List of strings
         */
        STRLIST
    };
    // clang-format on
private:
    FieldTypeVals value;

public:
    FieldType() = default;
    constexpr FieldType(FieldTypeVals val) : value(val) {}
    FieldType(const std::string& value) {

        if (StrUtil::istrEquals(value, "string")) {
            this->value = FieldType::STRING;
        } else if (StrUtil::istrEquals(value, "number")) {
            this->value = FieldType::NUMBER;
        } else if (StrUtil::istrEquals(value, "strlist")) {
            this->value = FieldType::STRLIST;
        } else if (StrUtil::istrEquals(value, "date")) {
            this->value = FieldType::DATE;
        } else {
            throw "Critical error: the attribute \"" + value + "\" doesn't have a valid field type";
        }
    }

    constexpr bool operator==(FieldType& b) {
        return this->value == b.value;
    }
    constexpr bool operator!=(FieldType& b) {
        return this->value != b.value;
    }

    operator FieldTypeVals() const {
        return value;
    }

    explicit operator bool() = delete;
};

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
     * Whether or not this field can be modified.
     */
    bool modifiable = true;

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
    std::optional<nlohmann::json> allowedValues;

public:
    Attribute() {}
    virtual ~Attribute() = default;

    /**
     * This method is used to modify or insert a specific field into the given task.
     *
     * This method has to be called in order to properly modify the field type.
     *
     * The default implementation can also be used by overriding classes. The method
     * itself does NOT change any data, but runs basic data validation that spreads
     * across all the different attributes.
     *
     * At the time of writing, this means a check to see if the change is an
     * attempted modification if we're not allowing modifications.
     * If there's an attempted modification on a non-modifiable field, the
     * method throws.
     */
    virtual void modify(nlohmann::json& task, const std::string& input) {
        if (!this->modifiable && task.find(this->name) != task.end()) {
            throw "The field " + this->name + " cannot be modified";
        }

        if (this->type != FieldType::STRLIST) {
            if (!isValueAllowed(input)) {
                throw "The value " + input + " is not allowed for " + this->name;
            }
        }
    }

    /**
     * Runs the relevant checks against the field
     */
    void checkField(nlohmann::json& attribValue);

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

    FieldType getType() {
        return type;
    }

    /**
     * Returns whether a given value is allowed for the field or not.
     *
     * Always returns true if there's no value limitation.
     * Otherwise, returns whether the value is contained in the set
     * of @{allowedValues}
     *
     * Note that the input is a JSON object to enable type compat.
     */
    bool isValueAllowed(const nlohmann::json& input);

    bool isBuiltin() {
        return builtin;
    }
    void setUda() {
        builtin = false;
    }

    bool hasLimitedValues() {
        return allowedValues.has_value();
    }

    static std::shared_ptr<Attribute> createAttrib(const std::string& fieldName);

    friend std::shared_ptr<Attribute> UDAAttribute::makeAttribute(const std::string& attributeName);
};

} // namespace taskranger
