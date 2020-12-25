# Taskranger

[![Build Status (Travis)](https://travis-ci.org/LunarWatcher/Taskranger.svg?branch=master)](https://travis-ci.org/LunarWatcher/Taskranger) [![Build status (Appveyor)](https://ci.appveyor.com/api/projects/status/8e8c0tqi4fp85e15?svg=true)](https://ci.appveyor.com/project/LunarWatcher/taskranger)

Taskranger is yet another CLI-based task manager.

This is created as an alternative to Taskwarrior, with changes to some of the core task management principles in mind. This is also written entirely from scratch, in pure C++ (looking aside the build system and package manager) to make sure it offers exactly the right experience.

**Status**: Usable, lacks a significant amount of features.

## Q&A

### Why make another task manager?

Because I had several problems with Taskwarrior that were labeled wontfix, or issues that have been around for a long time without being fixed. Still loving the idea behind Taskwarrior, why not make something that works for me?

Also because the vast majority of the other task managers out there either lack flexibility (Any.do, Todoist, etc.), or ended up being too simple (todo.txt).

### Why code from scratch?

Because rewriting code (as in forking and modifying) will arguably take more time than rewriting from scratch. Additionally, it lets me rewrite mechanics from scratch, instead of reworking code I don't understand how works.

### Why is the documentation so hard to understand?

It's not supposed to -- if it's hard to understand, that's my fault. I mainly write documentation for technical purposes, and not for end-users of a program with potentially no experience in programming and/or no experience with JSON (the data format used for config).

If any part of the documentation is too complicated, techy, or just weird, open an issue and I'll try to fix it. The same applies if something is outright missing. Pull requests are also welcome.

That being said, keep in mind that Taskranger is still far from ready for a stable release. As a result, certain parts will be missing simply because I haven't gotten around to it. Issues on missing documentation are still welcome. Documentation is also on the roadmap to v1.0.0, which is why criticism of documentation is important. Criticism means less shitty documentation, and yes, I'm more than willing to delay the v1.0.0 release (when that's an approaching reality) if the documentation is complete and utter garbage.

### I don't like the input format

At the moment, the input parser is fairly basic, in part because I've never written anything like it before. Additionally, I don't have a complete overview of all the things I'd like to include. Regardless of syntax changes, however, a major goal is to avoid ambiguity. The parser shouldn't have to fill in the gaps and guess what you mean - finding a balance between easy to read and write syntax, as well as syntax that isn't ambiguous is tricky, to say the least. The current syntax tries to achieve some balance, but it's not completely optimal either.

There's going to be changes to the input syntax at some point:tm:, but that is a job for after v1.0.0.

## Dev setup

The only requirements for this project are:

* A C++17 compiler
* Python 3.5+
* [SCons](https://scons.org/) and [Conan](https://conan.io) (`python3 -m pip install -r requirements.txt` on all platforms, `. ./init.sh` to automate install and virtualenvs on Linux/Mac)

Additionally, the use of `<filesystem>` adds additional requirements to the stdlib
* Linux (GCC, Clang): libstdc++-8-dev or newer
* Linux, Mac (Clang): libc++-7-dev or newer
* Windows (MSVC): MSVC STL 15.7 or newer.
    **Note**: PATH cannot contain Git Bash unless msys2 takes precedence over Git Bash - this causes ICU's conan package (ICU is one of Taskranger's dependencies) to detect msys2 incorrectly. Unfortunately, at least in the failing CI tests, this seems to cause the package to not download msys or use a functioning msys, but rather use Git Bash. That triggers build failures which presumably won't happen with proper msys, or if there's no msys-like bash to interfere with ICU. TL;DR: if Git Bash is in your path, the build will likely fail. See KnownIssues.md#windows-icu-fails-to-compile


The stdlib makes or breaks the code. Also note that even if your compiler is C++17 compliant, the stdlib might not be up to date and cause compilation to fail anyway.

---

While I currently only compile and build on Linux with Clang, it should be able to run on other operating systems with other compilers. If not, open an issue and I'll look into it.

VirtualEnv is highly recommended for installing SCons and Conan on Linux, and especially if your distro pushes two versions of Python on you. Run `. ./init.sh` ([the leading dot is important](https://stackoverflow.com/a/16011496/6296561)) to set it up automatically, or do it yourself. The point being, you need both SCons and Conan in a working state to build the project.

Conan grabs the other dependencies.

## Goals
* Ease of use: explicit syntax to avoid ambiguities in input. Caveat: potentially boilerplate
* Modern C++
* Working with as many workflows/methodologies as possible, if not all.

<i>Made with &lt;3 and Vim</i>
