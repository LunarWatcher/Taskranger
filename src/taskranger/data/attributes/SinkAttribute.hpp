#pragma once

#include "taskranger/data/Attribute.hpp"

namespace taskranger {

class SinkAttribute : public Attribute {
public:
    SinkAttribute(const std::string& name) {
        this->name = name;
        this->type = FieldType::STRING;
    }

    /**
     * Overload for non-string sinks
     */
    SinkAttribute(const std::string& name, FieldType& type) {
        this->name = name;
        this->type = type;
    }

    void modify(nlohmann::json&, const std::string&, const std::string&) override {}
    void validate(nlohmann::json&, nlohmann::json&) override {}
};

} // namespace taskranger
