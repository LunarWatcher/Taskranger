#pragma once

#include "taskranger/data/Attribute.hpp"

namespace taskranger {

class ULLongAttribute : public Attribute {
public:
    ULLongAttribute() {
        this->type = FieldType::ULLONG;
    }

    virtual void validate(nlohmann::json& attribValue) override {
        if (!attribValue.is_number() || attribValue.is_number_float() || !attribValue.is_number_unsigned()) {
            throw this->name + " only accepts unsigned numbers.";
        }
    }

    virtual void modify(nlohmann::json& task, const std::string& input) override;
};

class StringAttribute : public Attribute {
public:
    StringAttribute() {
        this->type = FieldType::STRING;
    }

    virtual void validate(nlohmann::json& attribValue) override {
        if (!attribValue.is_string()) {
            throw this->name + " only accepts strings.";
        }
    }

    virtual void modify(nlohmann::json& task, const std::string& input) override;
};

class StrListAttribute : public Attribute {
public:
    StrListAttribute() {
        this->type = FieldType::STRLIST;
    }

    virtual void validate(nlohmann::json& attribValue) override {
        if (!attribValue.is_array()) {
            // Only checking for strings could cause some problems, but we'll treat everything as a
            // string, so it should be fine.
            throw this->name + " only accepts string arrays";
        }
    }

    virtual void modify(nlohmann::json& task, const std::vector<std::string>& input);
    virtual void modify(nlohmann::json& task, const std::string& input) override;
};

class DateAttribute : public Attribute {
public:
    DateAttribute() {
        this->type = FieldType::DATE;
    }

    virtual void validate(nlohmann::json& /* attribValue */) override {
        // TODO + conversion (?)
    }

    virtual void modify(nlohmann::json& task, const std::string& input) override;
};

} // namespace taskranger
