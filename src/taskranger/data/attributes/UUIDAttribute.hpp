#pragma once

#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/attributes/AttribTypes.hpp"

namespace taskranger {

class UUIDAttribute : public StringAttribute {
public:
    UUIDAttribute() {
        this->name = "uuid";
        this->label = "UUID";
    }
};

class IDAttribute : public ULLongAttribute {
public:
    IDAttribute() {
        this->name = "id";
        this->label = "ID";
    }
};

} // namespace taskranger
