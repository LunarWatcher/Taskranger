#pragma once

#include "taskranger/data/Attribute.hpp"

namespace taskranger {

class ULLongAttribute : public Attribute {
public:
    ULLongAttribute() {
        this->type = FieldType::ULLONG;
    }
};

class StringAttribute : public Attribute {
public:
    StringAttribute() {
        this->type = FieldType::STRING;
    }
};

class StrListAttribute : public Attribute {
public:
    StrListAttribute() {
        this->type = FieldType::STRLIST;
    }
};

class DateAttribute : public Attribute {
public:
    DateAttribute() {
        this->type = FieldType::DATE;
    }
};

} // namespace taskranger
