#pragma once

#include "tabulate/table.hpp"
#include "taskranger/data/Task.hpp"
#include <string>
#include <variant>
#include <vector>

namespace taskranger {
namespace Types {

typedef std::variant<std::string, const char*, tabulate::Table> TableRow;
typedef std::vector<std::shared_ptr<Task>> TaskPtrVec;
} // namespace Types
} // namespace taskranger
