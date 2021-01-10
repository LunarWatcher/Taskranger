#include "RecurAttribute.hpp"
#include "taskranger/data/JSONDatabase.hpp"
#include "taskranger/util/DatetimeUtil.hpp"

namespace taskranger {

void RecurAttribute::update(Task& task /*template  task*/, JSONDatabase& database) {
    if (task.getOrElse<std::string>("tasktype", "no") != "rtemplate") {
        // Safeguard to prevent recur tasks from spawning recur tasks
        return;
    }
    // Covers the recur logic
    auto dueIt = task.getTaskJson().find("due");
    if (dueIt == task.getTaskJson().end()) {
        std::cout << std::string("ERROR: Recurring task doesn't contain a due date. UUID ") +
                             task.getTaskJson().at("uuid").get<std::string>() + " is invalid.\n";
        return;
    }
    auto due = task.getTaskJson().at("due").get<double>();
    auto now = DateTimeUtil::currTime();

    auto initialized = task.getOrElse("init", false);

    if (initialized && due > now) {
        // Nothing to worry about; the due date is still in the future.
        return;
    }
    auto recur = DateTimeUtil::parseRelative(task.getTaskJson().at("recur").get<std::string>(), 0);
    double newDue = due;

    // Loop to prevent lack of use from interfering with recurring tasks
    do {
        newDue += recur;
    } while (newDue < now);

    auto newTask = std::make_shared<Task>(&database);
    task >> *newTask;
    task.set("init", true);

    newTask->set("due", newDue);
    newTask->set("parent", task.getOrElse<std::string>("uuid", "unknown"));

    task.set("due", newDue);

    task.commitChanges();
    newTask->commitChanges();
    database.getDatabase().push_back(newTask);
    database.commit();
}

void RecurAttribute::modify(nlohmann::json& task, const std::string& key, const std::string& input) {
    double relParse = DateTimeUtil::parseRelative(input);
    if (relParse <= 0) {
        throw std::string("Failed to parse relative date for recurrence: ") + input;
    }
    task[key] = input;
    task["tasktype"] = "rtemplate";
}

void RecurAttribute::validate(nlohmann::json& /* value */, nlohmann::json& task) {
    using namespace std::literals;
    if (task.find("due") == task.end()) {
        throw "Must have a due date"s;
    }
}

} // namespace taskranger
