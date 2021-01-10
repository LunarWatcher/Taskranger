#pragma once
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <variant>

namespace taskranger {

class Config {
private:
    nlohmann::json config;

public:
    Config();

    void ensureLoaded();
    void loadStandards();

    std::string getString(const std::string& key, const std::string& defaultValue = "");
    unsigned long long getULLong(const std::string& key);
    bool getBool(const std::string& key);

    auto findKey(const std::string& key) {
        return config.find(key);
    }

    auto begin() {
        return config.begin();
    }

    auto end() {
        return config.end();
    }

    size_t size() {
        return config.size();
    }

    const nlohmann::json& getConfig() {
        return config;
    }
    const nlohmann::json& getConfig() const {
        return config;
    }
};

} // namespace taskranger
