#pragma once

#include "nlohmann/json.hpp"
#include "taskranger/data/attributes/AttribTypes.hpp"
#include "taskranger/metadata/Types.hpp"

namespace taskranger {

class DueAttribute : public DateAttribute {
public:
    DueAttribute() {
        this->name = "due";
        this->label = "Due";
    }
};

} // namespace taskranger
