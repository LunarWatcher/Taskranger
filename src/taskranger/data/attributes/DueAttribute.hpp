#pragma once

#include "taskranger/data/attributes/AttribTypes.hpp"

namespace taskranger {

class DueAttribute : public DateAttribute {
public:
    DueAttribute() {
        this->name = "due";
        this->label = "Due";
    }
};

} // namespace taskranger
