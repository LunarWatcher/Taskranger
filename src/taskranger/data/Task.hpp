#pragma once

#include "taskranger/input/operators/InputParserOperators.hpp"
#include <functional>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <variant>

namespace taskranger {
namespace Task {

/**
 * Defines the types of various attributes. Example:
 *   description: string
 *   created: date
 */
//clang-format off
static std::map<std::string, std::string> attribTypeMap = {
        {"description", "string"}, {"tags", "strlist"}, {"project", "string"}};
//clang-format on
void loadUserDefinedAttribs();

void convertAndEval(InputParserOperators::Operator op, const std::string& fieldName, const std::string& rawInput,
        nlohmann::json& reworked);

} // namespace Task
} // namespace taskranger
