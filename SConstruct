from SConsStandard import getEnvironment
import os
from sys import platform
import SCons

EnsureSConsVersion(3, 1, 2)
EnsurePythonVersion(3, 5)

env = getEnvironment(
    customVariables=[PathVariable("prefix", "Defines an install prefix. Only used when running `scons install`.", "/")]
)

# ================= Begin docs ====================
env.addHelp("""
scons install       installs Taskranger if you're on Mac, Linux, or Windows
                    with /usr/bin.

scons               Builds the binary.

scons test          Runs tests. Should be given the same arguments as SCons
                    if you want to prevent recompiling of certain parts.
                    Otherwise, it rebuilds Taskranger itself in a different
                    configuration.
""")

env.addHelp("""
scons supports certain variables to modify the build.
These are added by invoking scons with variable=value.
Example:

    scons debug=no

The variables are:
""")
env.addVariableHelp()
# ================== End docs =====================

if "install" in BUILD_TARGETS:
    if platform == "win32" and not os.path.isdir("/usr/bin"):
        print("""I'm not even gonna pretend like I know how to make this work.
Unlike UNIX, there's no single place to just dump binaries and have them
interface with the system properly. It'll end up requiring a manual path
addition, which I don't think I can do without writing large amounts of
windows-only code.

Consequentially, I have no idea how to write an install script, because
it'd only end up being partial anyway.

If you want to see scripted install on Windows, you'll have to submit a
pull request.
""")
        env.environment.Alias("install")
    else:
        print("Installing to {}".format(env.environment["prefix"] + "/usr/bin"))
        ins = Install(
            env.environment["prefix"] + "/usr/bin",
            env.environment.File(os.path.join(
                    env.path, "src/bin", "taskranger"
                )
            ))
        env.environment.Alias("install", ins)
else:

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
