
# PaymentChannel

A C++ payment channel library for doing off-chain micropayments. It has no refund transaction, and uses CSV instead, and also supports multiway channels in a single contract transaction.

conan package name: `PaymentChannel/0.1.2@joystream/stable`

The package is published to joystream bintray repository, which can be accessed by adding a conan remote.

`conan remote add joystream-bintray https://api.bintray.com/conan/joystream/joystream True`

You can also use the package by cloning the git repo.

### Dependencies

This library, has *immediate* dependencies

- [common-cpp](https://github.com/JoyStream/common-cpp)
- [gtest](https://github.com/google/googletest/)


and they are managed using [Conan](https://conan.io), a platform and build system agnostic C++ package manager.

## Running Unit Tests

```
sh run_tests.sh
```

### Example Usage

When working with a local copy of the sources, the simplest way is to export the package to the testing channel, `PaymnetChannel/0.0.0@joystream/testing`. Inside the `conan_package/` directory, run the test_package command, this will export and do a test build of the package:

```
cd conan_package/
conan test_package
```

Next we need to install the library and it's dependencies into our consuming project. Move into the `example/` directory and run:

```
cd example/
conan install ./ --build=missing
```

This will produce an appropriate `conanbuildinfo.cmake` file, used in the building of the library itself, which is used in the example
project CMakeLists.txt to find the library. Now we can build our project:

```
cmake .
cmake --build .
```

And finally run the built example:
```
bin/example
```

## Exporting specific version locally
By cloning the repo there is no need to rely on a remote conan repo. This is done by exporting a specific tag

```
git checkout v0.1.2
cd conan_package/
conan export joystream/stable --file release_include_source.py
```

## License & Copyright

JoyStream paymentchannel library is released under the terms of the MIT license.
See [LICENSE](LICENCE) for more information.
