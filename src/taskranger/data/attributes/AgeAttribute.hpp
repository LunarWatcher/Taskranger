#pragma once

#include "taskranger/data/attributes/AttribTypes.hpp"

namespace taskranger {

class AgeAttribute : public DateAttribute {
public:
    AgeAttribute() {
        this->name = "age";
        this->label = "Created";
    }

    virtual Types::TableRow getMinimalRepresentationForTable(const Task& task) override;
};

} // namespace taskranger
