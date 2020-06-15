#pragma once

#include "taskranger/util/StrUtil.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace taskranger {
namespace InputParserOperators {

// clang-format off
enum class Operator {
    IS, // ==
    NOT, // !=
    GREATER_THAN, // >
    GREATER_EQ, // >=
    LESS_THAN, // <
    LESS_EQ, // <=
    CONTAINS
};
// clang-format on

/**
 * Determines and splits the operator string from the input.
 * For:
 *   ids.not
 * It returns:
 *   {Operator::NOT, "ids"}
 *
 * This should in theory deal with multi-key inputs (i.e. some.input.not),
 * but this isn't implemented and possibly unsupported.
 */
std::pair<Operator, std::string> determineOperator(const std::string& attribKey);

/**
 * Evaluates an `input` by comparing it to a `relativeTo` using
 * a binary `Operator op`.
 *
 * Because different fields support different types of operators,
 * this is the base function specialized for numbers and booleans.
 * Strings have a template specialization
 *
 * Supported operators:
 * Numbers and bools:
 *  - IS
 *  - NOT
 *  - GREATER
 *  - LESS
 * Strings:
 *  - IS (case-insensitive)
 *  - NOT (case-insensitive)
 *  - CONTAINS
 * Vector<*> (searches items):
 *  - IS (strings: case-intensive)
 *  - NOT (strings: case-intensitive)
 */
template <typename T>
bool evalOperator(Operator op, const T& input, const T& relativeTo) {
    switch (op) {
    case Operator::IS:
        return input == relativeTo;
    case Operator::NOT:
        return input != relativeTo;
    case Operator::GREATER_THAN:
        return relativeTo > input;
    case Operator::GREATER_EQ:
        return relativeTo >= input;
    case Operator::LESS_THAN:
        return relativeTo < input;
    case Operator::LESS_EQ:
        return relativeTo <= input;
    default:
        return false;
    }
}

template <>
bool evalOperator(Operator op, const std::string& input, const std::string& relativeTo);

template <typename G>
bool evalOperator(Operator op, const std::vector<G>& input, const std::vector<G>& relativeTo) {
    switch (op) {
    case Operator::IS:
        for (auto& a : input) {
            bool found = false;
            for (auto& b : relativeTo) {
                if constexpr (std::is_same<G, std::string>::value) {
                    if (StrUtil::istrEquals(a, b)) {
                        found = true;
                    }
                } else if (a == b) {
                    found = true;
                }
            }
            if (!found)
                return false;
        }
        return true;
    case Operator::NOT:
        for (auto& a : input) {
            for (auto& b : relativeTo) {
                if constexpr (std::is_same<G, std::string>::value) {
                    if (StrUtil::istrEquals(a, b)) {
                        return false;
                    }
                } else if (a == b) {
                    return false;
                }
            }
        }
        return true;
    default:
        return false;
    }
}

} // namespace InputParserOperators
} // namespace taskranger
