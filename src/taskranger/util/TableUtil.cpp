#include "TableUtil.hpp"
#include "tabulate/color.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <algorithm>

namespace taskranger {

tabulate::Table TableUtil::renderTasks(
        std::vector<std::shared_ptr<Task>>& renderTarget, std::map<std::string, int> keyPriority) {
    // TODO: Stress-test this in actual cases and cap the size
    tabulate::Table table;
    // clang-format off
    // Clear the edges for a slightly slicker feel.
    // (Borders don't make sense for this specific table)
    table.format()
        .border_top("")
        .border_bottom("")
        .border_left(" ")
        .border_right(" ")
        .corner("");
    // clang-format on

    std::vector<TableRow> keys;

    for (auto& task : renderTarget) {
        for (auto& [k, v] : task->getTaskJson().items()) {
            // Because std::variant doesn't define an operator==, this is a hack to work around
            // that.
            if (std::find_if(keys.begin(), keys.end(), [&key = k](const auto& arg) {
                    // The following exception should never be thrown.
                    if (!std::holds_alternative<std::string>(arg))
                        throw std::runtime_error("This is why we can't have nice things");
                    return std::get<std::string>(arg) == key;
                }) == keys.end()) {
                keys.push_back(k);
            }
        }
    }

    // Sorts the keys by priority. Unprioritized ones are sorted slightly randomly, but hopefully consistently.
    // possibly stupid code
    std::sort(keys.begin(), keys.end(), [&keyPriority = keyPriority](const auto& rawA, const auto& rawB) -> bool {
        auto a = std::get<std::string>(rawA);
        auto b = std::get<std::string>(rawB);

        auto itA = keyPriority.find(a);
        auto itB = keyPriority.find(b);
        if (itA == keyPriority.end() && itA == itB)
            return false;
        int valA, valB;
        if (itA != keyPriority.end())
            valA = itA->second;
        else
            valA = 5;
        if (itB != keyPriority.end())
            valB = itB->second;
        else
            valB = 5;
        return valA < valB;
    });

    table.add_row(keys);
    // Modify the description to stay narrow.
    // This might need to be tweaked based on terminal width.
    // See TermUtils.hpp for a function that gets the cols (width)
    // of the terminal.
    for (auto& cell : table.row(0)) {
        if (cell.get_text() == "description") {
            cell.format().width(60);
            break;
        }
    }

    table[0].format().font_style({tabulate::FontStyle::underline});

    for (auto& taskObj : renderTarget) {
        auto& task = taskObj->getTaskJson();
        std::vector<TableRow> row;
        for (auto& key : keys) {
            auto value = task.find(std::get<std::string>(key));
            if (value == task.end()) {
                row.push_back("");
            } else {
                row.push_back(StrUtil::toString(*value));
            }
        }
        table.add_row(row);
    }
    // Minor hack; the index is required, but the table class doesn't expose
    // any sizes, or have index-based access.
    size_t idx = 0;
    for (auto& row : table) {
        // grey is black according to the source code.
        // Stupid as fuck, but what can ya do?
        if (idx != 0 && idx % 2 == 0)
            row.format().background_color(tabulate::Color::white).font_color(tabulate::Color::grey);
        idx++;
    }
    return table;
}

} // namespace taskranger
