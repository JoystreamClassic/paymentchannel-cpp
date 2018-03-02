PROJECT(PackageTest)
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

ADD_EXECUTABLE(example example.cpp)
TARGET_LINK_LIBRARIES(example ${CONAN_LIBS})
