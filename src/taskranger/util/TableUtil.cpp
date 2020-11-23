#include "TableUtil.hpp"
#include "tabulate/color.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/util/StrUtil.hpp"
#include "taskranger/util/TermUtils.hpp"
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

void TableBuilder::build(const std::vector<std::shared_ptr<Task>>& tasks) {
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
        .width(TermUtils::getWidth())
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
        this->fixWidth(table);

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
            this->fixWidth(taskTable);

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

void TableBuilder::fixWidth(tabulate::Table& table) {
    size_t termWidth = (size_t)TermUtils::getWidth();

    auto currDim = table.shape();

    // Avoid processing if the table width is smaller than the terminal. We don't want
    // to do work on something that already works fine.
    auto currWidth = currDim.first;
    if (currWidth <= termWidth || termWidth <= 0)
        return;

    // We need to resize, so let's resize
    // first, determine the number of columns
    auto cols = table.getColumns();
    if (cols == 0) {
        std::cerr << "WARNING: getColumns() returned 0." << std::endl;
        return;
    }
    // Adjust for borders in the table
    termWidth -= cols;
    // Adjust for edges
    termWidth -= 2;

    std::vector<size_t> widths(cols, std::floor(termWidth / cols));

    // Cache for columns, since table.column(i) regenerates the class every time.
    // Since this function is at least O(n^2), caching has a certain impact.
    std::vector<tabulate::Column> colObjs;
    for (size_t i = 0; i < cols; i++) {
        colObjs.push_back(table.column(i));
    }

    size_t sum = 0, old = 0;

    // Stores columns that're considered "optimum"
    std::vector<size_t> optimum;
    // Init run
    for (size_t i = 0; i < cols; i++) {
        auto& col = colObjs.at(i);

        size_t termColsPerEntry = std::floor(termWidth / cols);
        size_t colWidth = col.get_computed_width();
        auto targetWidth = std::min(colWidth, termColsPerEntry);

        if (targetWidth == colWidth) {
            optimum.push_back(i);
        }
        sum += targetWidth;
        widths[i] = targetWidth;
    }

    while (sum < termWidth && optimum.size() < cols) {
        size_t termColsPerEntry = std::floor((termWidth - sum) / (cols - optimum.size()));

        for (size_t i = 0; i < cols; i++) {
            if (std::find(optimum.begin(), optimum.end(), i) != optimum.end()) {
                continue;
            }

            auto& col = colObjs.at(i);
            size_t colWidth = col.get_computed_width();

            size_t currWidth = widths.at(i);
            if (colWidth > currWidth) {
                auto toAdd = termColsPerEntry;
                // Only add what's necessary; if termColsPerEntry is more than
                // the column needs, figure out how much it needs and just add
                // that.
                if (currWidth + termColsPerEntry > colWidth) {
                    toAdd = colWidth - currWidth;
                }
                widths[i] = currWidth + toAdd;
                if (widths[i] >= colWidth) {
                    optimum.push_back(i);
                }
                sum += toAdd;
            } else {
                optimum.push_back(i);
            }
        }

        // Easy convergence; in the event we can't get an increase, but optimum isn't
        // equal to the number of columns, and the sum isn't equal to the term width,
        // consider us finished and stop
        if (sum == old)
            break;
        old = sum;
    }

    for (size_t i = 0; i < cols; i++) {
        table.column(i).format().width(widths.at(i));
    }
}

} // namespace TableUtil

} // namespace taskranger
