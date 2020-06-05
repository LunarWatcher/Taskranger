#pragma once

#include "taskranger/data/attributes/AttribTypes.hpp"
namespace taskranger {

class TagsAttribute : public StrListAttribute {
public:
    TagsAttribute() {
        this->label = "Tags";
    }
};

} // namespace taskranger
