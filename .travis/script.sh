if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
    CMAKE_URL="https://cmake.org/files/v3.18/cmake-3.18.4-Linux-x86_64.tar.gz"
    mkdir cmake && travis_retry wget -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
    sudo rm -f $(which cmake) # Try to tell the old version to get the fuck out
    export PATH=$(pwd)/cmake/bin:${PATH}
else
    brew install cmake
fi

cmake --version

. ./init.sh
conan profile new --detect default
scons -j 2 debug=False
scons -j 2 debug=False test
