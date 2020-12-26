#pragma once

#include "taskranger/data/attributes/AttribTypes.hpp"
namespace taskranger {

class TagsAttribute : public StrListAttribute {
public:
    TagsAttribute() {
        this->name = "tags";
        this->label = "Tags";
    }
};

class VTagsAttribute : public StrListAttribute {
public:
    VTagsAttribute() {
        this->name = "vtags";
        this->label = "Virtual tags";
    }
};

} // namespace taskranger
