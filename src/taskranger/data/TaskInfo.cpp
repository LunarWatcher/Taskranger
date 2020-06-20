#include "TaskInfo.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <memory>
#include <string>
#include <variant>

namespace taskranger {

void TaskInfo::convertAndEval(InputParserOperators::Operator op, const std::string& fieldName,
        const std::string& rawInput, std::vector<std::shared_ptr<Task>>& output,
        std::vector<std::shared_ptr<Task>>& input) {
    using namespace std::literals;
    auto attribute = Environment::getInstance()->getAttribute(fieldName);
    if (attribute == nullptr) {
        throw "Invalid attribute: " + fieldName + " (debug: .2)";
    }

    // Note: the result from evalOperator has to be inverted in this specific function.
    // This is because it returns whether or not it's true, but remove_if removes
    // if the call is true. The bool therefore has to be inverted.
    auto type = attribute->getType();
    switch (type) {
    case FieldType::STRING:
        TaskInfo::insItems(output, input, fieldName, op, rawInput);
        break;
    case FieldType::STRLIST: {
        auto strVec = StrUtil::splitString(rawInput, ",");
        TaskInfo::insItems(output, input, fieldName, op, strVec);
    } break;
    case FieldType::ULLONG:
        try {
            size_t idx = 0;
            auto num = std::stod(rawInput, &idx);
            if (idx == rawInput.length()) {
                TaskInfo::insItems(output, input, fieldName, op, num);
                break;
            }

        } catch (std::invalid_argument&) { //
            // If this exception is thrown, continue to the UUID check
        } catch (std::out_of_range&) { //
            return; //
        }
        if (rawInput.length() > 36) {
            break;
        }

        for (size_t i = 0; i < input.size(); i++) {
            auto& task = input.at(i);
            auto uuid = task->getUUID();
            if (uuid == rawInput || StrUtil::startsWith(uuid, rawInput)) {
                output.push_back(task);
                break;
            }
        }

        break;
    case FieldType::NUMBER:
        try {
            size_t idx = 0;
            auto num = std::stod(rawInput, &idx);
            if (idx == rawInput.length()) {
                TaskInfo::insItems(output, input, fieldName, op, num);
            }
        } catch (std::invalid_argument&) { //
            return; //
        } catch (std::out_of_range&) { //
            return; //
        }
        break;
    default:
        throw "This type hasn't been implemented yet."s;
    }
}

} // namespace taskranger
