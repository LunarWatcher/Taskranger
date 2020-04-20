# Taskranger

Taskranger is yet another CLI-based task manager.

(DISCLAIMER: the program hasn't come far enough to actually do anything meaningful. Come back in 6-8)

This is created as an alternative to Taskwarrior, with changes to some of the core task management principles in mind. This is also written entirely from scratch, in pure C++ (looking aside the build system and package manager) to make sure it offers exactly the right experience.

## Q&A

* Q: Why make another task manager?
A: Because I had several problems with Taskwarrior that were labeled wontfix, or issues that have been around for a long time without being fixed. Still loving the idea behind Taskwarrior, why not make something that works for me?

Also because the vast majority of the other task managers out there either lack flexibility (Any.do, Todoist, etc.), or ended up being too simple (todo.txt).

* Q: Why code from scratch?
A: Because rewriting code (as in forking and modifying) will arguably take more time than rewriting from scratch. Additionally, it lets me rewrite mechanics from scratch, instead of reworking code I don't understand how works.

## Dev setup

The only requirements for this project are:

* A C++17 compiler
* Python 3
* [SCons](https://scons.org/) and [Conan](https://conan.io)

While I currently only compile and build on Linux with Clang, it should be able to run on other operating systems with other compilers. If not, open an issue and I'll look into it.

VirtualEnv is highly recommended for installing SCons and Conan on Linux. Run `. ./init.sh` ([the leading dot is important](https://stackoverflow.com/a/16011496/6296561)) to set it up automatically, or do it yourself. The point being, you need both SCons and Conan in a working state to build the project.

Conan grabs the other dependencies.
