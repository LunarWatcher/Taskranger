#include "nlohmann/json.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/metadata/Types.hpp"

namespace taskranger {

class RecurAttribute : public Attribute {
public:
    RecurAttribute() {
        this->name = "recur";
        this->label = "Recurring";
        this->label = "How often the task will recur";
    }

    void modify(nlohmann::json& task, const std::string& key, const std::string& input) override;
    Types::TableRow getMinimalRepresentationForTable(const nlohmann::json& task) override;
    Types::TableRow getMaxRepresentationForTable(const nlohmann::json& task) override;
};

} // namespace taskranger
