import os
import time
import sys

print("========== Bootstrapper ==========")
print("Initializing submodules...")
# Init submodules
os.system("git submodule update --init --recursive")

print("\n----------------------------------\nStart config")
venv = input("Do you want to use a virtualenv? [yes/no]: ").lower()[0] == "y"
print(f"{'Using' if venv else 'Not using'} virtualenv. Ctrl-C now if you want to abort. (Waiting 5 seconds...)")
time.sleep(5)

if venv:
    # Install virtualenv (system, user if not sudo)
    os.system("pip3 install virtualenv")

    if not os.path.isdir("env"):
        # Create a virtualenv
        os.system("virtualenv env")

    # Install the requirements in the virtualenv
    os.system("./env/bin/pip install -r requirements.txt")

    if input("Do you also wish to install SCons system-wide? (Required for `sudo scons install`) [yes/no]: ").lower() == "yes":
        os.system("python3 -m pip install --upgrade scons")
else:
    os.system("python3 -m pip install -r requirements.txt")


print("""-----------------------------------
Bootstrap complete.
If you're a Linux user, you'll additionally have to `sudo pip3 install scons`, if you intend
on installing. This is because the virtualenv copy of SCons isn't made available
directly. Additionally, with standard root config, you'll have to install SCons with:

    sudo pip install scons

... where sudo may or may not be another keyword, depending on your distro

If you used a virtualenv, please activate it now before continuing.""")
