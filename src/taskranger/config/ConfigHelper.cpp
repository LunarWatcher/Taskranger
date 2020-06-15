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
        try {
            input >> config;
        } catch (nlohmann::json::parse_error& err) {
            std::cout << err.what() << std::endl;
            throw "The JSON parser ran into an error when parsing the config JSON. Please make sure your JSON file is "
                  "valid";
        }
    }
}

std::string Config::getString(const std::string& key) {
    return this->config.value(key, "");
}

unsigned long long Config::getULLong(const std::string& key) {
    return this->config.value(key, 0);
}

} // namespace taskranger
