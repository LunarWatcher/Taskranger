#include "AttribTypes.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/metadata/Types.hpp"
#include <stdexcept>

namespace taskranger {

class DependencyAttribute : public StrListAttribute {
public:
    DependencyAttribute() {
        this->name = "depends";
        this->label = "Depends on (blocked by)";
    }

    void modify(nlohmann::json& task, const std::string& key, const std::string& input) override;

    Types::TableRow getMaxRepresentationForTable(const nlohmann::json& task) override;
    Types::TableRow getMinimalRepresentationForTable(const nlohmann::json& task) override;
};

} // namespace taskranger
