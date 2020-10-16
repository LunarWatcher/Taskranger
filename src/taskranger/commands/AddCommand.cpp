#include "AddCommand.hpp"
#include "tabulate/table.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/data/attributes/AttribTypes.hpp"
#include "taskranger/data/attributes/TagsAttribute.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/DatetimeUtil.hpp"
#include "taskranger/util/UIDUtils.hpp"
#include <memory>

namespace taskranger {

AddCommand::AddCommand() {
    this->name = "add";
    this->help = "Adds a new task.";
    this->usage = "taskranger add <description> <attributes>";
}

void AddCommand::run() {
    using namespace std::literals;

    auto input = Environment::getInstance()->getInputData();
    auto& data = input->data;
    if (data.find("description") == data.end()) {
        throw "You need to add a message to the task."s;
    }

    auto& database = *Environment::getInstance()->getDatabase("active.json", true);
    nlohmann::json mod;
    if (data.find("subcommand") != data.end()) {
        data.erase("subcommand");
    }

    // deal with IDs
    // The "reference" ID is the one that will be used in most common tasks. This ID
    // disappears when the task is archived. This is because UUIDs are a lot uglier,
    // so they're not as easy to type out as a basic number.
    // The IDs will also be compacted, so if task 1, 2, and 3 exist, and 2 is removed,
    // the former task 3 becomes task 2. This being the add command does not deal with that.
    // It counts the amount of items and generates an ID equivalent to the size plus 1.
    // This also means IDs aren't zero-indexed
    // As a bonus, IDs aren't explicitly declared. This variable exists for documentation,
    // and for the "created task" comment. The ID is actually computed from the task's position in the vector.
    size_t id = database.size() + 1;
    // But this is pretty useless after, and for various post-completion purposes, the tasks
    // also get a unique identifier.
    // TODO: sanity-check the UUID to prevent a UUID collision check.
    // On the other hand, this might be completely unnecessary, with about a 50% chance
    // of at least one collision per 2.71*10^18 UUIDs. A third option is implementing a
    // fallback strategy on commands relying on the UUID. The odds it gets used are insanely
    // low, but better safe than sorry on this one.
    std::string uuid = uuid::generateUuidV4();

    data["uuid"] = uuid;
    auto now = DateTimeUtil::currTime();
    data["created"] = "RAW" + std::to_string(now);
    Environment& env = *Environment::getInstance();
    if (input->tags.size() != 0) {
        std::transform(input->tags.begin(), input->tags.end(), input->tags.begin(),
                [](const std::string& rawTag) { return rawTag.at(0) == '-' ? "+" + rawTag.substr(1) : rawTag; });
        std::dynamic_pointer_cast<TagsAttribute>(env.getAttribute("tags"))->modify(mod, input->tags);
    }
    mod["modified"] = now;

    for (auto& [key, value] : data) {
        auto attrib = env.getAttribute(key);
        if (!attrib) {
            throw "Attribute doesn't exist: " + key;
        }
        attrib->modify(mod, key, value);
        // Has to be attrib->name to prevent issues with scoped
        // attributes (like dates)
        attrib->validate(mod.at(attrib->getName()));
    }
    // TODO at a later point: add the time of the task's creation
    (*database.getRawDatabase()).push_back(mod);
    database.commit();
    std::cout << "Successfully created task " << id << ".\n";
}

} // namespace taskranger
