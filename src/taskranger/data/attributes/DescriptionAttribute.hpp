#pragma once

#include "AttribTypes.hpp"

namespace taskranger {

class DescriptionAttribute : public StringAttribute {
public:
    DescriptionAttribute() {
        this->name = "description";
        this->label = "Description";
    }
};

} // namespace taskranger
