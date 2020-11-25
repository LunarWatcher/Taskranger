#include "TaskFilter.hpp"
#include "taskranger/data/Attribute.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/DatetimeUtil.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <algorithm>
#include <cstring>
#include <ostream>
#include <stdexcept>
#include <string>

namespace taskranger {

namespace TaskFilter {

std::vector<std::shared_ptr<Task>> Filter::filterTasks(const std::vector<std::shared_ptr<Task>>& tasks) {
    std::vector<std::shared_ptr<Task>> output;
    for (auto& task : tasks) {
        auto taskJson = task->getTaskJson();
        for (auto& filter : filters) {
            bool hasMatch = false;

            switch (filter->fieldType) {
            case FieldType::ULLONG:
                for (auto& value : filter->inputs) {
                    if (value.type() == typeid(std::string)) {
                        if (filter->fieldName != "id") {
                            throw "Accidentally ended up with a string passed as an ullong on a field that isn't the "
                                  "ID";
                        }
                        std::string sValue = std::any_cast<std::string>(value);
                        auto uuid = taskJson.at("uuid");
                        auto& op = filter->op;
                        auto startsWith = StrUtil::startsWith(uuid, sValue);
                        if ((op == InputParserOperators::Operator::IS && startsWith) ||
                                (op == InputParserOperators::Operator::NOT && !startsWith) ||
                                TaskFilter::checkTask(filter->op, "uuid", taskJson, sValue)) {

                            hasMatch = true;
                            break;
                        }
                    } else {

                        if (value.type() != typeid(unsigned long long)) {
                            throw "Somehow ended up with a non-ullong to an ullong field";
                        }
                        unsigned long long ullValue = std::any_cast<unsigned long long>(value);
                        if (TaskFilter::checkTask(filter->op, filter->fieldName, taskJson, ullValue)) {
                            hasMatch = true;
                            break;
                        }
                    }
                }
                if (!hasMatch) {
                    task->noMatch();
                }
                break;
            case FieldType::NUMBER:
            case FieldType::DATE:
                for (auto& value : filter->inputs) {
                    if (value.type() != typeid(double)) {
                        throw "Somehow ended up with a non-double passed to a number field";
                    }
                    double dValue = std::any_cast<double>(value);
                    if (checkTask(filter->op, filter->fieldName, taskJson, dValue)) {
                        hasMatch = true;
                        break;
                    }
                }
                if (!hasMatch) {
                    task->noMatch();
                }
                break;
            case FieldType::STRING: {
                auto sValue = std::any_cast<std::string>(filter->inputs.at(0));
                if (!TaskFilter::checkTask(filter->op, filter->fieldName, taskJson, sValue)) {
                    task->noMatch();
                }
            } break;
            case FieldType::STRLIST: {
                std::vector<std::string> vec;
                for (auto& value : filter->inputs) {
                    if (value.type() != typeid(std::string)) {
                        throw "Somehow ended up with a non-string to a strlist field";
                    }

                    vec.push_back(std::any_cast<std::string>(value));
                }
                if (!TaskFilter::checkTask(filter->op, filter->fieldName, taskJson, vec)) {
                    task->noMatch();
                }
            } break;
            }
            // Early finish; if the filter didn't match the task, break out
            if (!task->isIncludedInFilter()) {
                break;
            }
        }

        // If, however, we break out and the task is still in the game, let's include
        // it in the output.
        if (task->isIncludedInFilter()) {
            output.push_back(task);
        }
    }

    return output;
}

Filter& Filter::disableConditionally(const std::string& key, const std::string& value) {
    for (auto& fa : this->filters) {
        if (fa->fieldName == key) {
            for (auto& item : fa->inputs) {
                // Op-independent: item overridden
                if (std::any_cast<std::string>(item) == value) {
                    return *this;
                }
            }
            // op-specific: add
            if (fa->op == InputParserOperators::Operator::NOT) {
                fa->inputs.push_back(value);
                return *this;
            }
        }
    }

    // No neg op: add
    auto filter = std::make_shared<FilterInfo>();
    filter->op = InputParserOperators::Operator::NOT;
    filter->fieldType = FieldType::STRLIST;
    filter->inputs = {value};
    filter->fieldName = key;
    filters.push_back(filter);

    return *this;
}

Filter Filter::createFilter(std::shared_ptr<InputData> input) {
    using namespace std::literals;

    auto filters = input->data;
    if (input->tags.size() != 0) {
        std::string builder;
        std::string subBuilder;
        for (size_t i = 0; i < input->tags.size(); i++) {
            auto tag = input->tags.at(i);
            if (tag.at(0) == '-') {
                subBuilder += (subBuilder.length() != 0 ? ","s : ""s) + "+" + tag.substr(1);
            } else {
                builder += (builder.length() != 0 ? ","s : ""s) + tag;
            }
        }
        if (builder != "") {
            filters["tags"] = builder;
        }
        if (subBuilder != "") {
            filters["tags.not"] = subBuilder;
        }
    }

    Filter filterObj;
    // Filter the tasks
    for (auto& filter : filters) {
        auto baseKey = filter.first;
        if (baseKey == "subcommand") {
            continue;
        }
        std::shared_ptr<Attribute> attribPtr;
        auto calcPair = InputParserOperators::determineOperator(baseKey, attribPtr);

        InputParserOperators::Operator op = calcPair.first;
        const std::string& key = calcPair.second;
        std::string filterValue = filter.second;

        // For now: skip empty filters.
        // Will have to revisit this in the future
        if (filterValue.size() == 0) {
            continue;
        }

        if (key == "project" && filterValue.at(0) != '@') {
            filterValue = "@" + filterValue;
        }

        auto filterInfo = std::make_shared<FilterInfo>();
        filterInfo->fieldName = key;
        filterInfo->op = op;
        filterInfo->fieldType = attribPtr->getType();

        if (attribPtr->getType() == FieldType::DATE) {
            double timestamp = 0;
            if (StrUtil::startsWith(filterValue, "RAW")) {
                size_t pos;
                try {
                    timestamp = std::stod(filterValue.substr(3), &pos);
                } catch (std::invalid_argument&) { throw "Failed to parse " + filterValue.substr(3) + " as a number."; }
            } else {
                auto split = StrUtil::splitString(key, ".");
                filterInfo->fieldName = split.front();

                timestamp = DateTimeUtil::parseTimeWith(split.size() == 2 ? split.back() : "default", filterValue);
            }
            filterInfo->inputs.push_back(timestamp);
        } else {

            switch (attribPtr->getType()) {
            case FieldType::STRING:
                filterInfo->inputs.push_back(filterValue);
                break;
            case FieldType::STRLIST: {
                auto commaSplit = StrUtil::splitString(filterValue, ",");

                for (auto& str : commaSplit) {
                    filterInfo->inputs.push_back(str);
                }
            } break;
            case FieldType::ULLONG: {
                auto commaSplit = StrUtil::splitString(filterValue, ",");
                for (auto& str : commaSplit) {

                    try {
                        size_t idx = 0;
                        auto num = std::stoull(str, &idx);
                        if (idx == str.length()) {
                            filterInfo->inputs.push_back(num);
                            continue;
                        }

                    } catch (std::invalid_argument&) { //
                        // If this exception is thrown, continue to the UUID check
                    } catch (std::out_of_range&) {}

                    if (attribPtr->getName() == "id") {
                        if (str.length() > 36) {
                            continue;
                        }
                        // Assume UUID
                        filterInfo->inputs.push_back(str);
                    }
                }
            } break;
            case FieldType::NUMBER: {

                auto commaSplit = StrUtil::splitString(filterValue, ",");
                for (auto& str : commaSplit) {

                    try {
                        size_t idx = 0;
                        auto num = std::stod(str, &idx);
                        if (idx == filterValue.length()) {
                            filterInfo->inputs.push_back(num);
                            continue;
                        }

                    } catch (std::invalid_argument&) { //
                        // If this exception is thrown, continue to the UUID check
                    } catch (std::out_of_range&) {}
                }
            } break;
            default:
                throw "This type hasn't been implemented yet"s;
            }
        }
        filterObj.filters.push_back(filterInfo);
    }

    return filterObj;
}

} // namespace TaskFilter

} // namespace taskranger
