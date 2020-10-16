from SConsStandard import EnvMod
import os
from sys import platform
import SCons

EnsureSConsVersion(3, 1, 2)
EnsurePythonVersion(3, 5)

env = EnvMod.getEnvironment()
env.withConan()

database = None

if env.environment._get_major_minor_revision(SCons.__version__)[0] < 4:
    print("Note: Not including compilation database. Update to SCons 4.0.0")
    database = None
else:
    database = env.withCompilationDB()

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

if platform == "win32":
    # Required for proper unit testing
    env.includeSysVars("USERPROFILE", "HOMEDRIVE", "HOMEPATH")

# Run the executable submodule
taskranger = env.SConscript("src/SConscript",
               variant_dir="src",
               duplicate = 0,
               exports = {"database": database})

# lpath: contains the link path for libtaskranger
testEnv = env.Clone()
testEnv.SConscript("tests/SConscript",
                   variant_dir="tests",
                   duplicate=0,
                   exports = {"lpath": env.getBinPath(), "database": database})
