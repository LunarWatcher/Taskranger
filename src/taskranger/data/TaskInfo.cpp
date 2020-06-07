#include "TaskInfo.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <memory>
#include <string>
#include <variant>

namespace taskranger {

void Task::loadUserDefinedAttribs() {
    // Look at me, I'm an empty function!
    // TODO: implement
}

void Task::convertAndEval(InputParserOperators::Operator op, const std::string& fieldName, const std::string& rawInput,
        nlohmann::json& reworked) {
    using namespace std::literals;
    if (Environment::getInstance()->getAttribute(fieldName) == nullptr) {
        throw "Invalid attribute: " + fieldName;
    }

    // Note: this silently fails the exceptions, but doesn't filter.
    // clang-format off
#define NUMCATCH(func) \
    try { \
        auto num = func(rawInput); \
        Task::eraseItems(reworked, fieldName, op, num); \
    } catch (std::invalid_argument) { \
        return; \
    } catch (std::out_of_range) { \
        return; \
    }
    // clang-format on

    // Note: the result from evalOperator has to be inverted in this specific function.
    // This is because it returns whether or not it's true, but remove_if removes
    // if the call is true. The bool therefore has to be inverted.
    auto type = Environment::getInstance()->getAttribute(fieldName)->getType();
    switch (type) {
    case FieldType::STRING:
        Task::eraseItems(reworked, fieldName, op, rawInput);
        break;
    case FieldType::STRLIST: {
        auto strVec = StrUtil::splitString(rawInput, ",");
        Task::eraseItems(reworked, fieldName, op, strVec);
    } break;
    case FieldType::ULLONG:
        NUMCATCH(std::stoull)
        break;
    default:
        throw "This type hasn't been implemented yet."s;
    }
#undef NUMCATCH
}

} // namespace taskranger
