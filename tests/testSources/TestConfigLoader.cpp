#include "catch2/catch.hpp"

#include "taskranger/config/ConfigHelper.hpp"
#include "taskranger/data/Environment.hpp"

#include "taskranger/input/EnvVars.hpp"
#include "util/SetEnv.hpp"

TEST_CASE("Load with env", "[EnvLoader]") {

    TestUtil::localSetenv("TASKRANGER_CONFIG_LOCATION", "./tests/raw/LoaderTest.trconf");
    REQUIRE(taskranger::Env::getEnv("TASKRANGER_CONFIG_LOCATION") == "./tests/raw/LoaderTest.trconf");
    auto config = taskranger::Environment::createInstance()->getConfig();
    REQUIRE(config->getString("flag") == "success!");
}
