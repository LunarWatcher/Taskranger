#include "AttribTypes.hpp"
#include "date/date.h"
#include "taskranger/data/Environment.hpp"
#include <stdexcept>

namespace taskranger {

void NumberAttribute::modify(nlohmann::json& task, const std::string& input) {
    Attribute::modify(task, input);

    size_t convEndPos = 0;
    double value = 0;
    try {
        value = std::stod(input, &convEndPos);
    } catch (std::invalid_argument&) { convEndPos = 0; }

    if (convEndPos != input.length()) {
        throw "Invalid number: " + input;
    }

    task[this->name] = value;
}

void ULLongAttribute::modify(nlohmann::json& task, const std::string& input) {
    if (this->name == "id") {
        /**
         * Unmodifiable fields are better documented later, but the ID is an exception.
         * The ID itself is never actually stored in the JSON (or rather, it shouldn't be).
         *
         * As a result, the field isn't present, so in spite of the field not being
         * modifiable, it would be possible to modify it. As a result, it requires a special
         * case to catch ID modifications  before there's a conflicting field, because that
         * would turn into a minor problem pretty fast.
         *
         * That being said, there's nothing preventing ID overwrites in the data itself,
         * but that would be overwritten by the system when it computes the ID. When it
         * comes to being modified on the other hand, it's slightly harder.
         *
         * This method throws if the ID is attempted modified because I'm not a fan of silent
         * errors.
         */
        throw "You cannot modify the ID";
    }

    Attribute::modify(task, input);

    size_t convEndPos = 0;
    unsigned long long value = 0;
    try {
        value = std::stoull(input, &convEndPos);
    } catch (std::invalid_argument&) { convEndPos = 0; }

    if (convEndPos != input.length()) {

        throw "Invalid unsigned long: " + input;
    }

    task[this->name] = value;
}

void StringAttribute::modify(nlohmann::json& task, const std::string& input) {
    Attribute::modify(task, input);

    task[this->name] = input;
}

void StrListAttribute::modify(nlohmann::json& task, const std::string& input) {

    auto vec = StrUtil::splitString(input, ',');

    this->modify(task, vec);
}

void StrListAttribute::modify(nlohmann::json& task, const std::vector<std::string>& vec) {
    if (!this->modifiable && task.find(this->name) != task.end()) {
        throw "Cannot modify an unmodifiable attribute (" + this->name + ")";
    }
    if (!isValueAllowed(vec)) {
        throw "Value not allowed in field " + this->name;
    }
    task[this->name] = vec;
}

void DateAttribute::modify(nlohmann::json& task, const std::string& input) {
    Attribute::modify(task, input);

    auto config = Environment::getInstance()->getConfig();
}

} // namespace taskranger
