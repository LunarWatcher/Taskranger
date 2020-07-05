#pragma once

namespace taskranger {
namespace Types {

typedef std::variant<std::string, const char*, tabulate::Table> TableRow;

}
} // namespace taskranger
