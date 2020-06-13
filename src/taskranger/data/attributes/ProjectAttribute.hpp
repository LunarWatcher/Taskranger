#pragma once

#include "taskranger/data/attributes/AttribTypes.hpp"
namespace taskranger {

class ProjectAttribute : public StringAttribute {
public:
    ProjectAttribute() {
        this->name = "project";
        this->label = "Project";
    }
};

} // namespace taskranger
