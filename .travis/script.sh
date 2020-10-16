. ./init.sh
conan profile new --detect default
scons -j 2 debug=False
scons -j 2 debug=False test
