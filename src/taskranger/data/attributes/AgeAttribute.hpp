#pragma once

#include "taskranger/data/attributes/AttribTypes.hpp"

namespace taskranger {

class AgeAttribute : public DateAttribute {
public:
    AgeAttribute() {
        this->name = "created";
        this->label = "Created";
    }

    virtual Types::TableRow getMinimalRepresentationForTable(const nlohmann::json& task) override;
};

} // namespace taskranger
