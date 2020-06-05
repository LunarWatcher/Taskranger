#include "TaskInfo.hpp"
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
    if (Task::attribTypeMap.find(fieldName) == Task::attribTypeMap.end()) {
        ColorPrinter printer;
        printer << ANSIFeature::FOREGROUND << 9 << "Invalid attribute: " << fieldName << std::endl;
        return;
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
    auto type = Task::attribTypeMap.at(fieldName);
    if (type == "string") {
        Task::eraseItems(reworked, fieldName, op, rawInput);
    } else if (type == "strlist") {
        auto strVec = StrUtil::splitString(rawInput, ",");
        Task::eraseItems(reworked, fieldName, op, strVec);
    } else if (type == "ullong") {
        NUMCATCH(std::stoull)
    } else if (type == "number" || type == "integer" || type == "int") {
        // Under the hood, store numbers and ints as signed long longs.
        NUMCATCH(std::stoll)
    } else if (type == "float" || type == "double") {
        // float and double is stored as a long double, which may contain
        // 128 bits. Compiler and arch dependent: https://stackoverflow.com/a/3454586/6296561
        NUMCATCH(std::stold)
    } else {
        std::cout << "Unrecognized type: " << type << ". This should not happen." << std::endl;
    }

#undef NUMCATCH
}

} // namespace taskranger
