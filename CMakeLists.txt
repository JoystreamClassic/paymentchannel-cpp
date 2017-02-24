cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(PaymentChannel CXX)

option(build_tests "build tests" OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories("${CMAKE_SOURCE_DIR}/include")

set(library_sources
    src/Commitment.cpp
    src/ContractTransactionBuilder.cpp
    src/Payee.cpp
    src/Payor.cpp
    src/RedeemScript.cpp
    src/Refund.cpp
    src/Settlement.cpp
    src/UnspentBuyerRefundOutput.cpp)

# === build library ===
add_library(protocol_wire ${library_sources})

# === build tests ===
if(build_tests)
  file(GLOB tests RELATIVE "${PROJECT_SOURCE_DIR}" "test/test_*.cpp")

  enable_testing()

  foreach(s ${tests})
    get_filename_component (sn ${s} NAME_WE)
    add_executable(${sn} ${s})
    target_link_libraries(${sn} protocol_wire ${CONAN_LIBS})
    add_test(${sn} "bin/${sn}")
  endforeach(s)

endif()
