#pragma once
#include <map>
#include <memory>
#include <string>
#include <variant>

namespace taskranger {

class Config {
private:
    std::map<std::string, std::string> config;

public:
    Config();

    void ensureLoaded();
    void loadStandards();

    std::string getString(const std::string& key);
    unsigned long long getULLong(const std::string& key);
};

} // namespace taskranger
