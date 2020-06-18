from SConsStandard import EnvMod
import os
from sys import platform

env = EnvMod.getEnvironment()
env.withConan()

if "test" in BUILD_TARGETS:
    env.define("UNITTEST")

# Minor patch for asan
env.environment['ENV']['ASAN_OPTIONS'] = 'halt_on_error=0;detect_leaks=0'
# TODO: uncomment when the next version of SCons comes out
# See LunarWatcher/SConsStandard#4
# env.withCompilationDB()

if platform == "win32":
    env.includeSysVars("USERPROFILE", "HOMEDRIVE", "HOMEPATH")


# Run the executable submodule
env.SConscript("src/SConscript",
               variant_dir="src" if "test" not in BUILD_TARGETS else "testSrc",
               duplicate = 0)

# lpath: contains the link path for libtaskranger
testEnv = env.Clone()
testEnv.SConscript("tests/SConscript", "tests", duplicate=0, exports = {"lpath": env.getBinPath()})
