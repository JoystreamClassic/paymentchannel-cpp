#!/bin/bash

mkdir -p build
cd build
conan install ../conan_package/ --build=missing
cmake ../sources -Dbuild_tests=on
make
ctest
