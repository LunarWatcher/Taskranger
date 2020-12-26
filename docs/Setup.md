# Setup

Note: this file uses a mix of `python`, `python3`, `pip`, and `pip3`, but all four variants are meant to refer to Python 3 or pip for Python 3. Python 2 isn't supported, and you shouldn't use it in general (deprecated as of 2020).

Also note that certain bits may be off on permissions, because I build outside of sudo and install with sudo. Most of this is adapted to that.

## Environment setup (all operating systems)

### `bootstrap.py`
`bootstrap.py` can help you with virtualenvs and package installs, but it's more or less just a fancy wrapper around `git submodule update --init --recursive` and `pip install -r requirements.txt`. It's up to you whether you chose to use a virtualenv or not, but on Linux, it's a requirement to use `sudo scons install`. It's not required if you install to somewhere that isn't `/usr/bin` (use `scons install prefix=some_prefix` - `/usr/bin` is appended to the path. This may be changed if this is illogical or breaks with standard Make functionality).

Additional note about Linux and Mac: the script is built on the assumption that you have sudo on your system. [Sudo is admittedly not entirely universal](https://unix.stackexchange.com/a/48553/398922), but it's as good as it gets. Install manually otherwise.

Overcomplicated details aside: `bootstrap.py` is a Python wrapper around the manual steps. For non-Windows systems, it assumes you have `sudo` installed and functioning normally. Run `python3 bootstrap.py` to get started - it'll take care if the stuff you need. It'll also ask you whether you want to use virtualenvs or not at the start.

### Manual steps

If you didn't `git clone` with `--recursive`, you _have_ to `git submodule update --init --recursive`. This is to initialize the submodules, and everything else will fail if you don't do this. A noticeable symptom of this is that SConsStandard can't be resolved.

If you want to use a virtualenv, you'll need to `pip install virtualenv`. If you have access to a `/dev/bin` and you're on a system that requires root, you have to `sudo pip3 install scons`, or otherwise make `scons` available to the root user. If you feel like running the entire build in sudo, just using standard virtualenvs should be fine.

Activate your venv if you're using one. Install the requirements with `pip3 install -r requirements.txt`, optionally with sudo if you prefer.

## Building (all operating systems)
Note: only MSVC, Clang (and clang-cl on Windows), and GCC are supported. As for the rest, I either don't know of it, or have classified it as obsolete or obscure.

Before you can build, as mentioned in the README, you need to install SCons and Conan, as well as have a C++17-capable compiler.

The build process is straight-forward: run `scons debug=no` in the root directory of the project. As with other make-like tools, you can `scons -j<thread count> debug=no` to speed up the build. I recommend not exceeding the number of supported threads (check your CPU details for the number), as this will instead slow the build down.

See also `scons -h` for help with the SConstruct defined by Taskranger, as well as `scons -H` for help with SCons itself.

## Install (Linux, Mac, etc.)

Run `sudo scons install`, or just `scons install` if you're logged into a user with root access.

### Uninstall

There's currently no way to uninstall - this is planned for a later time. `sudo rm $(which taskranger)` takes care of the job in the meanwhile. This will be implemented before v1.0.0, and will either happen when I feel like it, or if more files are installed aside just the binary.

## Install (Windows)

### WSL

WSL is Linux.

### MinGW, Cygwin, or similar

See the instructions for Linux, but drop `sudo` - doesn't apply to Windows since the filesystem doesn't have a concept of permissions.

### Plain Windows

Sorry, there's no fancy way for you.

Due to how Windows works, there's no place to just dump the binary and have it work.

You can still build taskranger by running `scons` in your terminal, but moving it to a place covered by your PATH variable is something that isn't supported as a part of the script.

If, however, you have a portable way of making this work on different systems, a PR to the SConstruct is welcome.

## Known build issues

### SconsStandard not found, catch2 not found, tabulate not found, or some or all of those directories are empty

You forgot `git submodule update --init --recursive`.

### `sudo scons install` results in command not found

`scons` has to be installed in a way that's accessible for root or sudo. Virtualenvs aren't "passed down" through sudo, which means you'll have to install it system-wide with `sudo pip3 install scons`.

... or just jump into the root user, activate the virtualenv, and run `scons install`. Or possibly use some other trick I don't know of, in which case, an amendment to this section is more  than welcome.

### Uninstall

For WSL and MinGW et al, see the Linux uninstall instructions.

No automatic install means no automatic uninstall. Remove the binary from whereever you placed it.
