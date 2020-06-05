#pragma once

#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/attributes/AttribTypes.hpp"

namespace taskranger {

class UUIDAttribute : public Attribute {
public:
    UUIDAttribute() {
        this->label = "UUID";
    }
};

class IDAttribute : public ULLongAttribute {
public:
    IDAttribute() {
        this->label = "ID";
    }
};

} // namespace taskranger
