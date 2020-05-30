#include "ConfigHelper.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/input/EnvVars.hpp"
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
    if (this->config.size() != 0) {
        // this function is a lazy load function. If the data already has been initialized,
        // don't reinitialize.
        return;
    }
    std::string configBasePath = "~/.trconf";

    auto reassigned = Env::getEnv("TASKRANGER_CONFIG_LOCATION", configBasePath);
    if (reassigned != configBasePath && reassigned != "") {
        configBasePath = FilesystemUtil::joinPath(reassigned, ".trconf");
    }

    auto expandedPath = FilesystemUtil::expandUserPath(configBasePath);
    if (configBasePath.at(0) == '~' && expandedPath == configBasePath) {

        std::cout << "ABORT LOADING CONFIG! This should not happen. Please set the environment variable "
                     "\"TASKRANGER_CONFIG_LOCATION\""
                  << std::endl;
        throw std::runtime_error("FATAL: config path expansion failed.");
    }

    std::fstream input(expandedPath);
    loadStandards();

    if (input) {
        std::string line;
        while (std::getline(input, line)) {
            if (line.length() == 0 || line.at(0) == '#')
                continue;

            auto keyValuePair = StrUtil::splitString(line, "=", 1);
            if (keyValuePair.size() != 2) {
                std::cout << "Warning: invalid configuration line:\n" << line << std::endl;
                continue;
            }
            this->config[keyValuePair.at(0)] = keyValuePair.at(1);
        }
    }
}

std::string Config::getString(const std::string& key) {
    if (this->config.find(key) == this->config.end()) {
        return "";
    }
    return this->config.at(key);
}

unsigned long long Config::getULLong(const std::string& key) {
    if (this->config.find(key) == this->config.end()) {
        return 0;
    }
    return std::stoull(this->config.at(key));
}

} // namespace taskranger
