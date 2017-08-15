#!/bin/bash

mkdir -p build
cd build
conan install ../conan_pacakge/ --build=missing
cmake ../sources -Dbuild_tests=on
make
ctest
