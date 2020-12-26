#pragma once

#include "taskranger/data/attributes/UUIDAttribute.hpp"

namespace taskranger {

class ParentAttribute : public UUIDAttribute {
public:
    ParentAttribute() {
        this->name = "parent";
        this->label = "Parent";
        this->modifiable = false;
    }
};

} // namespace taskranger
