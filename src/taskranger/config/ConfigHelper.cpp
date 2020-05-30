#include "ConfigHelper.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/util/FilesystemUtil.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <fstream>
#include <string>

namespace taskranger {

Config::Config() {}

void Config::loadStandards() {
    // clang-format off
    config = {
        { "dataDir", "~/.taskranger" }
    };
    // clang-format on
}

void Config::ensureLoaded() {
    const static std::string configBasePath = "~/.trconf";
    if (this->config.size() != 0) {
        // this function is a lazy load function. If the data already has been initialized,
        // don't reinitialize.
        return;
    }

    auto expandedPath = FilesystemUtil::expandUserPath(configBasePath);
    if (expandedPath == configBasePath) {
        // If, for whatever reason, this fails, it means this approach is fundementally flawed
        // and needs to have a plan B.
        std::cout << "ABORT LOADING CONFIG! This should never happen - open an issue on GitHub: "
                  << "https://github.com/lunarwatcher/taskranger" << std::endl;
        throw std::runtime_error("FATAL: config path expansion failed.");
    }

    std::fstream input(expandedPath);
    loadStandards();

    if (input) {
        std::string line;
        while (std::getline(input, line)) {
            if (line.length() == 0 || line.at(0) == '#')
                continue;

            auto keyValuePair = StrUtil::splitString("=", 1);
            if (keyValuePair.size() != 2) {
                std::cout << "Warning: invalid configuration line:\n" << line << std::endl;
                continue;
            }
            this->config[keyValuePair.at(0)] = keyValuePair.at(1);
        }
    }
}

std::string Config::getString(const std::string& key) {
    return this->config.at(key);
}

unsigned long long Config::getULLong(const std::string& key) {
    return std::stoull(this->config.at(key));
}

} // namespace taskranger
