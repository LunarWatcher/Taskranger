[ $TRAVIS_OS_NAME = 'osx' ] && brew install python@3.8
. ./init.sh
conan profile new --detect default
scons -j 2 debug=False
scons -j 2 debug=False test
