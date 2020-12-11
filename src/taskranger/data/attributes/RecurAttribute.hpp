#include "nlohmann/json.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/data/Task.hpp"
#include "taskranger/data/attributes/AttribTypes.hpp"
#include "taskranger/metadata/Types.hpp"

namespace taskranger {

class RecurAttribute : public StringAttribute {
public:
    RecurAttribute() {
        this->name = "recur";
        this->label = "Recurring";
    }

    void update(Task& task, JSONDatabase& dest);

    void modify(nlohmann::json& task, const std::string& key, const std::string& input) override;
    void validate(nlohmann::json& attribValue, nlohmann::json& task) override;
};

} // namespace taskranger
