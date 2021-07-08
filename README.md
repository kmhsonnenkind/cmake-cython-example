# Example: Building C library with Cython Wrapper using CMake

[![GitHub](https://img.shields.io/github/license/kmhsonnenkind/cmake-cython-example)](https://github.com/kmhsonnenkind/cmake-cython-example/blob/main/LICENSE)
[![Travis (.com)](https://app.travis-ci.com/kmhsonnenkind/cmake-cython-example.svg?branch=main)](https://app.travis-ci.com/kmhsonnenkind/cmake-cython-example)
[![Codecov](https://codecov.io/gh/kmhsonnenkind/cmake-cython-example/branch/main/graph/badge.svg?token=5LNZWPDGI2)](https://codecov.io/gh/kmhsonnenkind/cmake-cython-example)

## About

This repository shows how to use [cmake](https://cmake.org/) to build a native `C` or `C++` library and (optionally) add a reusable [Python 3](https://www.python.org) wrapper on top using [cython](https://cython.org).


## Build and Run the Code

The build uses `cmake` to build both the native library as well as the `Python 3` bindings.

### Dependencies

As the build uses `cmake` first off you will need [cmake](https://cmake.org/) (*duh*).

Additionally you will need a `C` compiler supporting at least `C99` as well as a `C++` compiler supporting at least `C++11`. The build should work on any compiler but was tested with `gcc`, `clang` and `msbuild`.

For the `Python` bindings you will need to install a [CPython interpreter](https://www.python.org) supporting at least language level `3.6` alongside its dev tools.

On top of that you will need to install `cython` to transpile the provided `.pyx` files to `c++` and have them compiled for use in `Python`. For the Python tests you will need to install [pytest](https://docs.pytest.org). By default Python's build script will check if the requirements are installed and if not install them on the fly.

If you want to further get test coverage information for the native tests you will also need [lcov](https://github.com/linux-test-project/lcov).

Finally if you also want to build the API documentation you will need [doxygen](https://www.doxygen.nl/) for the native library as well as [sphinx](https://www.sphinx-doc.org/) for the Python bindings.

On `Debian` based systems you can install the required packages using:

```sh
sudo apt-get update
# Install required dependencies
sudo apt-get install build-essential \
                     cmake \
                     gcc \
                     g++

# Install optional dependencies
sudo apt-get install python3 \
                     python3-dev \
                     cython3 \
                     python3-pytest \
                     python3-pytest-runner \
                     doxygen \
                     python3-sphinx \
                     lcov
```

Otherwise all tools should also provide installers for your targeted operating system. Just follow the instructions on the tools' sites.

### Build

The build can be triggered like any other `cmake` project.

```sh
cmake ..
cmake --build .
```

It offers several configuration parameters described in the following sections. For your convenience a [cmake variants](https://vector-of-bool.github.io/docs/vscode-cmake-tools/variants.html) file is provided that lets you choose the desired target configuration.

#### Python Bindings

To enable the build of the Python bindings set the `BUILD_PYTHON_BINDINGS` cmake parameter to `ON`.

```sh
cmake -DBUILD_PYTHON_BINDINGS=ON ..
cmake --build .
```

This will set the required configuration parameters for the Python build by generating a [setup.cfg](https://docs.python.org/3/distutils/configfile.html) file based on the in- and output directories of the native library. The Python build can also work without `cmake` but you will need to make sure that Python can find the public headers for the `foo` library as well as the built library itself (e.g. by installing the native library).

Depending on the other `cmake` settings, enabling the Python build might also enable additional features (like the sphinx based API documentation if `BUILD_DOCUMENTATION` is set to `ON`).

#### Tests

Both the native library as well as the Python bindings are unittested. To enable the automatic build of the unittests you can set the `cmake` parameter `BUILD_TESTING` to `ON`. After this you can run the tests by using [ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html).

```sh
cmake --build .
ctest .
```

The tests for the native library are using [catch2](https://github.com/catchorg/Catch2) (provided in *tests/catch2*). The source code for the tests can be found in *tests*.

The Python bindings use [pytest](https://docs.pytest.org). The code can be found in *extras/python-bindings/tests*.

##### Coverage

If you want to to get detailed information about the code coverage of the native test cases you can turn on the `cmake` configuration option `CODE_COVERAGE` (`OFF` by default). This option is only available if `BUILD_TESTING` is also enabled.

You can then use [lcov](https://github.com/linux-test-project/lcov) to get detailed information.

```sh
# Build and run tests
cmake -DBUILD_TESTING=ON -DCODE_COVERAGE=ON ..
cmake --build .
ctest .

# Get code coverage
lcov --capture --directory . --output-file code.coverage
lcov --remove code.coverage '/usr/*' --output-file code.coverage
lcov --remove code.coverage '**/tests/*' --output-file code.coverage
lcov --remove code.coverage '**/catch.hpp' --output-file code.coverage
lcov --list code.coverage
```


#### Documentation

For further information on the API you can build additional documentation yourself. Use the `BUILD_DOCUMENTATION` flag when configuring `cmake` to add the custom target `documentation` to the build (also added to the `ALL` build).

If enabled the native API documentation will be built using [doxygen](https://www.doxygen.nl/) and the Python documentation will be built using [sphinx](https://www.sphinx-doc.org/).

```sh
cmake -DBUILD_DOCUMENTATION=ON ..
cmake --build . --target documentation
```

The native documentation will be put in a directory *docs* in `cmake`'s build directory. The documentation for the Python bindings will be put in the same directory under *python*.

## Use Code in Own Project

The code offers a native library that is meant to be included in your projects. To simplify the integration the repository is configured to be usable as either a `(git) submodule` or to be installable like any other `cmake` project.

### Use as (Git) Submodule

To use the native library as a git submodule simply clone it somewhere in your source tree (e.g. in an *external* directory) and use `add_subdirectory` in your *CMakeLists.txt* file.

```sh
git submodule init
git submodule add https://github.com/kmhsonnenkind/cmake-cython-example.git external/foo
```

In the *CMakeLists.txt* file you can then do something like:

```
project(bar)

add_subdirectory(external/foo)

add_executable(bar bar.c)
target_link_libraries(bar kmhsonnenkind::foo)
```

### Install to be Usable Outside of CMake

If you want to install the native library (as well as the Python bindings) you can also use the `cmake install` target (might require *superuser* privileges):

```sh
cmake --build .
sudo cmake --build . --target install
```

This will install:
* The native `foo` library (to somewhere like */usr/local/lib/*)
* The required headers for the `foo` library (to somewhere like */usr/local/include/*)
* The cmake files for `find_package` (to somewhere like */usr/local/lib/foo/*)
* (If enabled) The Python `foo` package (to somewhere like */usr/local/lib/python3.6/dist-packages/*)
