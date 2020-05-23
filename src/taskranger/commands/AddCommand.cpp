#include "AddCommand.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "tabulate/table.hpp"
#include "taskranger/util/UIDUtils.hpp"

namespace taskranger {

void AddCommand::run(std::shared_ptr<InputData> input) {
    auto& tokens = input->tokens;
    if (tokens.find("description") == tokens.end()) {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9
            << "You need to add a message to the todo.\n"
            << ANSIFeature::CLEAR;
        return;
    }

    JSONDatabase database("active.json");
    nlohmann::json mod = tokens;
    mod.erase("subcommand");

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
    int id = database.getDatabase()->size() + 1;
    // But this is pretty useless after, and for various post-completion purposes, the tasks
    // also get a unique identifier.
    // TODO: sanity-check the UUID to prevent a UUID collision check.
    // On the other hand, this might be completely unnecessary, with about a 50% chance
    // of at least one collision per 2.71*10^18 UUIDs. A third option is implementing a
    // fallback strategy on commands relying on the UUID. The odds it gets used are insanely
    // low, but better safe than sorry on this one.
    std::string uuid = uuid::generateUuidV4();

    mod["uuid"] = uuid;
    // Inject the project
    if (input->project != "")
        mod["project"] = input->project;
    if (input->tags.size() > 0)
        mod["tags"] = input->tags;

    // TODO at a later point: add the time of the task's creation
    (*database.getDatabase()).push_back(mod);
    database.commit();
    std::cout << "Successfully created task " << id  << ".\n";
}

}
