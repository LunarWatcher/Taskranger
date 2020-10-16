#pragma once

#include "taskranger/data/attributes/AttribTypes.hpp"

namespace taskranger {

/**
 * Metadata attribute; contains when the task was last modified.
 * Can be equal to the creation date if the task never has been
 * modified.
 */
class ModifiedAttribute : public DateAttribute {
public:
    ModifiedAttribute() {
        this->name = "modified";
        this->label = "Last modified";
        this->modifiable = false;
    }
};

} // namespace taskranger
