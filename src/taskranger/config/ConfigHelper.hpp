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

    std::string getString(const std::string& key);
    unsigned long long getULLong(const std::string& key);

    auto findKey(const std::string& key) {
        return config.find(key);
    }

    auto begin() {
        return config.begin();
    }

    auto end() {
        return config.end();
    }
};

} // namespace taskranger
