#include "SetEnv.hpp"
#include "catch2/catch.hpp"

#include "taskranger/data/Environment.hpp"

#define LOAD_CONFIG(confFile) \
    TestUtil::localSetenv("TASKRANGER_CONFIG_LOCATION", "./tests/raw/" confFile); \
    auto config = taskranger::Environment::getInstance(true)->getConfig(); \
    if (config->findKey("loaded") != config->end()) { \
        throw "Failed to load config"; \
    }\
