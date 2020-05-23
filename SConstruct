from SConsStandard import EnvMod
import os

env = EnvMod.getEnvironment()
env.withConan()

# Minor patch for asan
env.environment['ENV']['ASAN_OPTIONS'] = 'halt_on_error=0;detect_leaks=0'

# Run the executable submodule
libtaskranger = env.SConscript("src/SConscript", variant_dir="src", duplicate = 0)

tests = env.SConscript("tests/SConscript", "tests", duplicate=0, exports={"libtr": libtaskranger})
testAlias = Alias("test", [ tests ])
AlwaysBuild(testAlias)
