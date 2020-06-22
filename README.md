# Taskranger

![CI](https://github.com/LunarWatcher/Taskranger/workflows/CI/badge.svg)

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

## Dev setup

The only requirements for this project are:

* A C++17 compiler
* Python 3
* [SCons](https://scons.org/) and [Conan](https://conan.io) (`python3 -m pip install -r requirements.txt` on all platforms, `. ./init.sh` to automate install and virtualenvs on Linux/Mac)

Additionally, the use of `<filesystem>` adds additional requirements to the stdlib
* Linux (GCC, Clang): libstdc++-8-dev or newer
* Linux, Mac (Clang): libc++-7-dev or newer
* Windows (MSVC): MSVC STL 15.7 or newer

The stdlib makes or breaks the code. Also note that even if your compiler is C++17 compliant, the stdlib might not be up to date and cause compilation to fail anyway.

---

While I currently only compile and build on Linux with Clang, it should be able to run on other operating systems with other compilers. If not, open an issue and I'll look into it.

VirtualEnv is highly recommended for installing SCons and Conan on Linux, and especially if your distro pushes two versions of Python on you. Run `. ./init.sh` ([the leading dot is important](https://stackoverflow.com/a/16011496/6296561)) to set it up automatically, or do it yourself. The point being, you need both SCons and Conan in a working state to build the project.

Conan grabs the other dependencies.

<i>Made with &lt;3 and Vim</i>
