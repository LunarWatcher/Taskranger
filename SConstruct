from SConsStandard import EnvMod
import os
from sys import platform

env = EnvMod.getEnvironment()
env.withConan()
# Minor patch for asan
env.environment['ENV']['ASAN_OPTIONS'] = 'halt_on_error=0;detect_leaks=0'

# Run the executable submodule
libtaskranger = env.SConscript("src/SConscript", variant_dir="src", duplicate = 0)

# Enable transient system variables for the tests. Required on Windows,
# not required on Linux and Mac, but included anyway
env.includeSysVars(all=True)

tests = env.SConscript("tests/SConscript", "tests", duplicate=0, exports={"libtr": libtaskranger})
testAlias = env.environment.Alias("test", [ tests ], tests[0].abspath)
AlwaysBuild(testAlias)
