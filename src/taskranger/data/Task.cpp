#include "Task.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include "taskranger/util/ColorPrinter.hpp"
#include "taskranger/util/StrUtil.hpp"
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

    // Note: the result from evalOperator has to be inverted in this specific function.
    // This is because it returns whether or not it's true, but remove_if removes
    // if the call is true. The bool therefore has to be inverted.
    auto type = Task::attribTypeMap.at(fieldName);
    if (type == "string") {
        reworked.erase(std::remove_if(reworked.begin(), reworked.end(),
                               [op, rawInput, fieldName](const nlohmann::json& task) {
                                   auto missesField = task.find(fieldName) == task.end();
                                   if (op == InputParserOperators::Operator::NOT && missesField)
                                       return false;
                                   return missesField || !InputParserOperators::evalOperator(
                                                                 op, rawInput, task.at(fieldName).get<std::string>());
                               }),
                reworked.end());
    } else if (type == "strlist") {
        auto strVec = StrUtil::splitString(rawInput, ",");
        reworked.erase(std::remove_if(reworked.begin(), reworked.end(),
                               [op, strVec, fieldName](const nlohmann::json& task) {
                                   auto missesField = task.find(fieldName) == task.end();
                                   if (op == InputParserOperators::Operator::NOT && missesField)
                                       return false;
                                   return missesField || task.find(fieldName) == task.end() ||
                                          !InputParserOperators::evalOperator(
                                                  op, strVec, task.at(fieldName).get<std::vector<std::string>>());
                               }),
                reworked.end());
    }
}

} // namespace taskranger
