#include "ConfirmationHelper.hpp"
#include "taskranger/input/operators/InputParserOperators.hpp"
#include "taskranger/util/DatetimeUtil.hpp"
#include "taskranger/util/StrUtil.hpp"
#include "taskranger/util/TableUtil.hpp"
#include "taskranger/util/TermUtils.hpp"

namespace taskranger {

ConfirmationHelper::ConfirmationHelper(std::map<std::string, std::string> newData, Types::TaskPtrVec filteredTasks)
        : changes(newData), filteredTasks(filteredTasks) {
    this->environment = Environment::getInstance();
    this->bulkWarn = environment->getConfig()->findKey("warnings")->at("modify").at("bulk").get<size_t>();
}

void ConfirmationHelper::preprocessChanges() {
    nlohmann::json rawChanges;
    // Save some time by converting and validating the fields here
    for (auto& [attrib, change] : this->changes) {
        if (attrib == "tags") {
            // Specialization for tags; +tags will be added,
            // -tags will be removed if present
            auto tagVec = StrUtil::splitString(change, ',');

            for (auto& tag : tagVec) {
                if (tag.at(0) == '-') {
                    rawChanges["tags.remove"].push_back("+" + tag.substr(1));
                } else {
                    rawChanges["tags.add"].push_back(tag);
                }
            }
        } else {
            auto attribObj = this->environment->getAttribute(attrib);
            attribObj->modify(rawChanges, attrib, change);
        }
    }

    this->computedChanges = rawChanges;
}

void ConfirmationHelper::dumpChanges() {
    preprocessChanges();
    tabulate::Table table;
    // Explanation for the - 2: the width per cell needs to account for
    // padding, which seems (in manual tests) to correspond to
    // - 2.
    table.format().width((TermUtils::getWidth() - 2) / 2);
    // clang-format off
    table.format()
        .border_right(" ")
        .border_left(" ")
        .border_top("")
        .border_bottom("")
        .corner("");
    // clang-format on
    table.add_row({"Name", "Value"});

    size_t i = 1;
    for (auto& [k, v] : computedChanges.items()) {
        if (k == "tags.add") {
            std::cout << "The following tags will be added: "
                      << std::accumulate(v.begin(), v.end(), std::string(""),
                                 [](const std::string& last, const nlohmann::json& j) {
                                     return (last.size() == 0 ? "" : last + ", ") + j.get<std::string>();
                                 })
                      << std::endl;
        } else if (k == "tags.remove") {

            std::cout << "The following tags will be removed: "
                      << std::accumulate(v.begin(), v.end(), std::string(""),
                                 [](const std::string& last, const nlohmann::json& j) {
                                     return (last.size() == 0 ? "" : last + ", ") + j.get<std::string>();
                                 })
                      << std::endl;
        } else {
            auto attrib = this->environment->getAttribute(k);
            table.add_row({attrib->getLabel(), attrib->getMaxRepresentationForTable(computedChanges)});
            if (i % 2 == 0) {
                table[i].format().background_color(tabulate::Color::white).font_color(tabulate::Color::grey);
            }

            i++;
        }
    }
    std::cout << "after for" << std::endl;
    // the shape is (x, y). To check if there's more than one row (the header),
    // .second has to be used
    if (table.shape().second > 1ul) {
        // avoid printing empty tables
        table[0].format().font_style({tabulate::FontStyle::underline});

        std::cout << "The following metadata will be changed:\n\n" << table << std::endl;
    }
}

void ConfirmationHelper::commitChanges(std::shared_ptr<Task> task) {
    auto& json = task->getDatabase()->getRawDatabase()->at(task->getIdx());

    for (auto& [k, v] : computedChanges.items()) {
        if (k == "tags.add") {
            if (json.find("tags") == json.end()) {
                json["tags"] = std::vector<std::string>();
            }
            auto& tags = json["tags"];
            for (auto& tag : v) {
                if (std::find(tags.begin(), tags.end(), tag.get<std::string>()) == tags.end()) {
                    tags.push_back(tag);
                }
            }

        } else if (k == "tags.remove") {
            // No tags? No problem. Fuck this :D
            if (json.find("tags") == json.end()) {
                continue;
            }
            auto& tags = json["tags"];

            for (auto& tag : v) {
                if (tags.size() == 0) {
                    // Early abort if we have no or are out of tags
                    break;
                }
                auto itr = std::find(tags.begin(), tags.end(), tag.get<std::string>());
                if (itr != tags.end()) {
                    tags.erase(itr);
                }
            }
        } else {
            json[k] = v;
        }
    }
    json["modified"] = DateTimeUtil::currTime();
}

size_t ConfirmationHelper::process() {
    if (bulkWarn >= 0 && this->filteredTasks.size() > bulkWarn) {

        TableUtil::TableBuilder builder;
        if (this->filteredTasks.size() < 20) {
            std::cout << "The following tasks will be modified:\n\n";
            builder.withKeys({"Name", "Value"})
                    .withUniqueTables()
                    .setFilterKeys(false)
                    .setTransformKeys(false)
                    .build(this->filteredTasks);
        } else {
            std::cout << "You've selected " << this->filteredTasks.size()
                      << " tasks to edit. Because console IO can be slow, the task info per task will not be printed."
                      << " Make sure you review the changes carefully before committing them.\n";
        }
        std::cout << "The following changes will be made:\n\n";
        dumpChanges();
        std::cout << "Out\n" << std::endl;
        // TODO (language): Interactive might not be the best option here either. Works better than individual,
        // but still awkward
        std::cout << "Is this okay? ([Y]es/[n]o/[i]nteractive) ";
        std::string line;
        std::getline(std::cin, line);
        line = StrUtil::toLower(line);
        if (line.size() == 0) {
            line = "y";
        }
        switch (line.at(0)) {
        case 'y':
            for (auto& task : this->filteredTasks) {
                this->commitChanges(task);
            }
            return filteredTasks.size();
        case 'n':
            std::cout << "Aborting modifications..." << std::endl;
            return 0;
        case 'i': {
            bool changeRemaining = false;
            size_t count = 0;
            std::cout << "Entering interactive mode...\n";
            for (auto& task : this->filteredTasks) {
                TableUtil::TableBuilder builder;
                builder.withKeys({"Name", "Value"})
                        .withUniqueTables()
                        .setFilterKeys(false)
                        .setTransformKeys(false)
                        .build({task});
                bool legal = true;
                if (!changeRemaining) {
                    do {
                        legal = true;

                        std::cout << "Modify this task? ([Y]es/[n]o/[a]bort all/[c]hange remaining/[l]eave remaining) ";
                        std::getline(std::cin, line);
                        line = StrUtil::toLower(line);
                        if (line.size() == 0) {
                            // Optimization
                            line = "y";
                        }
                        switch (line.at(0)) {
                        case 'c':
                            changeRemaining = true;
                            count++;
                            this->commitChanges(task);
                            break;
                        case 'y':
                            count++;
                            this->commitChanges(task);
                            break;
                        case 'n':
                            // This pretty much only exists to continue the outer for loop.
                            break;
                        case 'a':
                            // The changes haven't been committed, so we return 0. 0 modified
                            // tasks signals to ModifyCommand that the changes should _not_ be committed
                            std::cout << "Aborting all changes...\n";
                            return 0;
                        case 'l':
                            // Leave the rest
                            return count;
                        default:
                            legal = false;
                            std::cout << "\nInvalid option: " + line + " - please enter a valid option.\n";
                            break;
                        }

                    } while (!legal);
                } else {
                    count++;
                    this->commitChanges(task);
                }
            }
            return count;
        } break;
        default:
            std::cout << "Invalid option specified (" << line << "); aborting changes.\n\n";
            return 0;
        }

        return 0;
    } else {
        this->preprocessChanges();
        for (auto& task : this->filteredTasks) {
            this->commitChanges(task);
        }
        return this->filteredTasks.size();
    }
}

} // namespace taskranger
