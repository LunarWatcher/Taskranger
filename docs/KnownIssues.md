#Known issues

## Newlines in the text breaks the table

... specifically, it disables the automatic word wrap, and causes weird behavior with the table. Support for newlines may be dropped entirely in a backwards compatible way.

**ETA**: None

## The table overflows if there's too much content

Happens because tabulate has no built-in size checks.

**ETA**: Before v1.0.0

## The program will break on devices where time_t is 32 bit in 2038
If you're still running 32 bit when the 2038 problem starts getting even closer, you should really update your hardware -- Taskranger breaking will be the tiniest problem

**Status**: wontfix

## Windows: ICU fails to compile

The ICU conan package is set up to detect MSYS2. If it detects MSYS2, in any shape, it will assume you have a working MSYS2 install, and skip the MSYS2 conan dependency. In theory, and I haven't tested because I don't use Windows, it should be fine with a proper MSYS2 install, but with a butchered version like Git Bash, it doesn't.<sub>If, for whatever reason, you can't get it to work with a full install of MSYS2 (read: not Git Bash), open an issue.</sub>

Some minor detection specs: [this function](https://github.com/conan-io/conan/blob/d40f2a9f644c390bfaf202a573d7fb999a30949f/conans/client/tools/oss.py#L390-L419) in Conan's tools is called from [ICU's conanfile](https://github.com/conan-io/conan-center-index/blob/master/recipes/icu/all/conanfile.py#L43-L46). If it detects MSYS2 in the PATH, it assumes it can get its hands on a working MSYS2 instance. Git for Windows, and I'm not entirely sure about the details, seems to be detected as MSYS2.

IIRC, Git Bash itself is based on MSYS2, which causes Conan's detection system to detect MSYS/-2 instead of Git Bash. While Git Bash is based on MSYS, it isn't MSYS. Somewhere in details I'm not entirely sure about (again, I don't use Windows, so I can't confirm any ideas on the matter), this causes the build to fail if it's run with Git Bash.

The CI itself struggled with this. That's why there's three lines of code removing all Git-related stuff from `PATH` before continuing with the build: https://github.com/LunarWatcher/Taskranger/blob/e82e22b8259c3dd064bef9b6b4e9ce9c6137a1bd/.github/workflows/main-ci.yml#L115-L117 (also added about 8 minutes to CI time per commit; the build itself will be faster on processors with more than 2 cores, however).

Note that you don't have to have MSYS2 installed; if you don't, there's a Conan package that's automatically pulled in by ICU. Note that it also pulls in MSYS2 if it detects Cygwin or WSL -- neither of these should present issues like the one introduced by Git Bash.

**Status**: wontfix
**Workaround**: remove Git Bash from the `PATH` environment variable
