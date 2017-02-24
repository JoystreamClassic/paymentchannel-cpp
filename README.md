
# paymentchannel-cpp

A C++ payment channel library for doing off-chain micropayments. It has no refund transaction, and uses CSV instead, and also supports multiway channels in a single contract transaction.

## Usage

`...coming later...`

## Build

### Dependencies

This library, and its test, has *immediate* dependencies

- [gtest](https://github.com/google/googletest/)
- [common-cpp](https://github.com/JoyStream/common-cpp)

and they are managed using [Conan](https://conan.io), a platform and build system agnostic C++ package manager. To build all dependencies, you can do:

```
mkdir build
cd build
conan install .. --build missing
```

which will produce an appropriate `conanbuildinfo.cmake` file, used in the building of the library itself.

### Building

In the same folder as the prior step you can do the following to build the library itself

```
cmake ..
cmake --build .
```

## Test

Set the `build_tests` options when generating the make file, hence:

```
cmake .. -Dbuild_tests=on
cmake --build .
```
_NB: Tests cannot to proper script validation, only signature validation, as the coincore (mSIGNA) has not built in script and interpeter support._

## Use

Easiest way is to consume conan recipe here (add later)

## License & Copyright

``...add later...`
