#include "catch2/catch.hpp"

#include "taskranger/config/ConfigHelper.hpp"
#include "taskranger/data/Environment.hpp"

#include "taskranger/util/EnvVariableHelper.hpp"
#include "util/SetEnv.hpp"

TEST_CASE("Load with env", "[EnvLoader]") {

    TestUtil::localSetenv("TASKRANGER_CONFIG_LOCATION", "./tests/raw/LoaderTest.trconf");
    REQUIRE(taskranger::EnvVariable::getEnv("TASKRANGER_CONFIG_LOCATION") == "./tests/raw/LoaderTest.trconf");
    auto config = taskranger::Environment::getInstance(true)->getConfig();
    REQUIRE(config->getString("flag") == "success!");
}
