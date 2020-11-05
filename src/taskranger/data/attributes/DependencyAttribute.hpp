#include "AttribTypes.hpp"

namespace taskranger {

class DependencyAttribute : public StrListAttribute {
    DependencyAttribute() {
        this->name = "depends";
        this->label = "Depends on";
    }
};

} // namespace taskranger
