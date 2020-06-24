from SConsStandard import EnvMod
import os
from sys import platform

env = EnvMod.getEnvironment()
env.withConan()

if "test" in BUILD_TARGETS:
    env.define("UNITTEST")

# Config {{{
def checkSecure(context):
    """
    Checks whether secure_getenv is present or not
    """
    context.Message("Checking secure_getenv... ")

    secureProbe = """
    #define _GNU_SOURCE
    #include <stdlib.h>
    int main() {
        secure_getenv("fakeVar");
    }
    """

    compiled = context.TryCompile(secureProbe, ".cpp")

    context.Result("yes" if compiled else "no")
    return compiled

def checkUncaughtExceptionS(context):
    context.Message("Checking for std::uncaught_exceptions... ")

    exceptProbe = """
    #include <exception>
    int main() {
        std::uncaught_exception();
    }
    """
    compiled = context.TryCompile(exceptProbe, ".cpp")
    context.Result("yes" if compiled else "no")
    return compiled

with env.configure() as config:
    config.test("CheckCXX")
    config.configureFilesystem()

    if platform != "win32":
        if config.test("CheckSecureGetEnv", callback = checkSecure):
            env.define("HAS_SECURE_GETENV")
    if config.test("CheckUncaughtExceptionS", callback = checkUncaughtExceptionS):
        env.define("HAS_UNCAUGHT_EXCEPTIONS")
# }}}

# Minor patch for asan
env.environment['ENV']['ASAN_OPTIONS'] = 'halt_on_error=0;detect_leaks=0'
# TODO: uncomment when the next version of SCons comes out
# See LunarWatcher/SConsStandard#4
# env.withCompilationDB()

if platform == "win32":
    env.includeSysVars("USERPROFILE", "HOMEDRIVE", "HOMEPATH")

# TODO(C++20): add a switch between using dependencies/date and
# C++20's stdlib.
# Using exports to SConscript would be cleanest; it also lets the dependencies folder
# actually deal with dependencies.
# This is TBD.
env.SConscript("dependencies/DateSConscript", variant_dir="date", duplicate=0)

# Run the executable submodule
env.SConscript("src/SConscript",
               variant_dir="src" if "test" not in BUILD_TARGETS else "testSrc",
               duplicate = 0)

# lpath: contains the link path for libtaskranger
testEnv = env.Clone()
testEnv.SConscript("tests/SConscript", "tests", duplicate=0, exports = {"lpath": env.getBinPath()})
