#pragma once

#include <nlohmann/json.hpp>
#include <vector>

namespace taskranger {

class JSONDatabase;
class Task {
private:
    /**
     * Rationale and optimization; each task holds a pointer to the über-tasklist
     * as well as its own ID in the array.
     *
     * Each of the different databases are used in a different location, which means
     * the output commands will be smart enough to figure out which have IDs,
     * and which don't.
     *
     * Moreover, this means no iteration has to be done when the database saves.
     * Writing straight to the über-database means that everything in the database
     * can be considered ready to write when it's time for that.
     *
     * Finally, the rest of this class means it's possible to store internal fields
     * without iterating to add them, and then remove them afterwards. This saves some
     * minor amount of clutter in the code
     */
    JSONDatabase* database;
    nlohmann::json taskJson;

    // fields computed internally that aren't written to disk
    // Keep in mind that some fields are computed on-demand to
    // to prevent wasting cycles, and therefore have a getter
    // instead of a field
    unsigned long long idx;

    bool includeInFilter{true};

    std::vector<std::shared_ptr<std::string>> dependsOn;

public:
    Task(JSONDatabase* taskList, unsigned long long idx);

    /**
     * Returns the ID, or 0 if no ID is found
     */
    unsigned long long getId();
    unsigned long long getId() const;

    const nlohmann::json& getTaskJson() const;
    bool hasPublicIds();
    std::string getUUID();
    template <typename T>
    T getOrElse(const std::string& key, const T& def) {
        auto& json = this->getTaskJson();
        auto itr = json.find(key);
        if (itr == json.end()) {
            return def;
        }
        return *itr->get<T>();
    }

    /**
     * This returns the ID for the task corresponding to a vector position.
     * DO NOT confuse getIdx for getId - these two are very different functions
     */
    unsigned long long getIdx() {
        return idx;
    }

    unsigned long long getIdx() const {
        return idx;
    }

    bool isIncludedInFilter() {
        return includeInFilter;
    }

    void initVTags();

    void noMatch() {
        this->includeInFilter = false;
    }

    void reset() {
        this->includeInFilter = true;
    }

    JSONDatabase* getDatabase() {
        if (this->database == nullptr) {
            throw "FATAL ERROR: Database pointer in task is nullptr";
        }
        return this->database;
    }
};

} // namespace taskranger
