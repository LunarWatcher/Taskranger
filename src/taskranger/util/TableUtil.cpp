#include "TableUtil.hpp"
#include "tabulate/color.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <algorithm>
#include <iostream>

namespace taskranger {

namespace TableUtil {

TableBuilder::TableBuilder() {}

TableBuilder& TableBuilder::withKeys(const std::vector<std::string>& keys) {
    this->keys = keys;
    for (auto& key : keys) {
        columns[key] = {};
    }
    return *this;
}

void TableBuilder::build(std::vector<std::shared_ptr<Task>>& tasks) {
    std::vector<Types::TableRow> keysWithValues;
    auto& env = *Environment::getInstance();
    if (this->filterKeys) {
        for (auto& taskPtr : tasks) {
            auto& task = *taskPtr;
            auto& json = task.getTaskJson();
            for (auto& key : keys) {
                auto itr = json.find(key);
                if (itr == json.end()) {
                    columns[key].push_back("");
                    continue;
                }
                std::shared_ptr<Attribute> attribPtr = nullptr;
                try {
                    attribPtr = env.getAttribute(key);

                } catch (std::string&) {}
                if (!attribPtr) {
                    columns[key].push_back(StrUtil::toString(*itr, " ", ""));
                } else {
                    columns[key].push_back(attribPtr->getMinimalRepresentationForTable(json));
                }
                // This system is used to make sure only keys with at least one value are printed.
                // The rest shouldn't be printed
                if (std::find_if(keysWithValues.begin(), keysWithValues.end(), [&key = key](const auto& arg) {
                        // The following exception should never be thrown.
                        if (!std::holds_alternative<std::string>(arg))
                            throw std::runtime_error("This is why we can't have nice things");
                        return std::get<std::string>(arg) == key;
                    }) == keysWithValues.end()) {
                    keysWithValues.push_back(key);
                }
            }
        }
    } else {
        // TODO: see if this can be optimized somehow
        for (auto& key : this->keys) {
            keysWithValues.push_back(key);
        }
    }

    tabulate::Table table;
    // clang-format off
    table.format()
        .border_top("")
        .border_bottom("")
        .border_left(" ")
        .border_right(" ")
        .corner("");
    // clang-format on
    std::vector<Types::TableRow> tfKeys;

    if (this->transformKeys) {
        for (auto& fuckingBullshitFormat : keysWithValues) {
            std::shared_ptr<Attribute> attribute = nullptr;
            std::string key = std::get<std::string>(fuckingBullshitFormat);
            try {
                attribute = env.getAttribute(key);
            } catch (std::string&) {}
            if (!attribute) {
                tfKeys.push_back(key);
                continue;
            }
            tfKeys.push_back(attribute->getLabel());
        }
        table.add_row(tfKeys);

        table[0].format().font_style({tabulate::FontStyle::underline});
    }

    if (this->filterKeys) {

        std::vector<std::vector<Types::TableRow>> rows;
        // Iterate keysWithValues to preserve order
        for (auto& key : keysWithValues) {

            auto& values = columns.at(std::get<std::string>(key));

            for (size_t i = 0; i < values.size(); i++) {
                if (i == rows.size()) {
                    rows.push_back({});
                }
                rows.at(i).push_back(values.at(i));
            }
        }

        for (auto& row : rows) {
            table.add_row(row);
        }
        this->fixBackground(table);

        // Limit the description size (temporary)
        // TODO: fix table sizing
        for (auto& cell : table.row(0)) {
            if (cell.get_text() == "Description") {
                cell.format().width(60);
                break;
            }
        }
        std::cout << table << std::endl;
    } else {
        for (auto task : tasks) {
            tabulate::Table taskTable;
            taskTable.format().border_top("").border_bottom("").border_left(" ").border_right(" ").corner("");

            taskTable.add_row(keysWithValues);
            taskTable[0].format().font_style({tabulate::FontStyle::underline});

            // https://github.com/nlohmann/json/issues/2040
            auto json = task->getTaskJson();
            for (auto& [k, v] : json.items()) {
                try {
                    auto attribute = env.getAttribute(k);
                    auto key = attribute->getLabel();
                    taskTable.add_row({key, attribute->getMaxRepresentationForTable(json)});
                } catch (std::string&) { taskTable.add_row({k, StrUtil::toString(v)}); }
            }

            this->fixBackground(taskTable);

            std::cout << taskTable << "\n\n";
        }
    }
}

void TableBuilder::fixBackground(tabulate::Table& table) {
    // Pretty background colors
    size_t idx = 0;
    for (auto& row : table) {
        // grey is black according to the source code.
        // Stupid as fuck, but what can ya do?
        if (idx != 0 && idx % 2 == 0)
            row.format().background_color(tabulate::Color::white).font_color(tabulate::Color::grey);
        idx++;
    }
}

} // namespace TableUtil

} // namespace taskranger
