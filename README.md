# Example: Building C library with Cython Wrapper using CMake

This repository shows how to use [cmake](https://cmake.org/) to build a native `C` or `C++` library and add a reusable [Python 3](https://www.python.org) wrapper on top using [cython](https://cython.org).


## Build and Run the Code

The build uses `cmake` to build both the native library as well as the `Python 3` bindings.

### Dependencies

As the build uses `cmake` first off you will need [cmake](https://cmake.org/) (*duh*).

Additionally you will need a `C` compiler supporting at least `C99` as well as a `C++` compiler supporting at least `C++11`. The build should work on any compiler but was tested with `gcc`, `clang` and `msbuild`.

For the `Python` bindings you will need to install a [CPython interpreter](https://www.python.org) supporting at least language level `3.6` alongside its dev tools.

On top of that you will need to install `cython` to transpile the provided `.pyx` files to `c++` and have them compiled for use in `Python`. For the Python tests you will need to install [pytest](https://docs.pytest.org). By default Python's build script will check if the requirements are installed and if not install them on the fly.

Finally if you also want to build the API documentation you will need [doxygen](https://www.doxygen.nl/) and [sphinx](https://www.sphinx-doc.org/).

On `Debian` based systems you can install the required packages using:

```sh
sudo apt-get update
sudo apt-get install build-essential \
                     cmake \
                     gcc \
                     g++ \
                     python3 \
                     python3-dev \
                     cython3 \
                     python3-pytest \
                     python3-pytest-runner \
                     doxygen \
                     python3-sphinx
```

Otherwise all tools should also provide installers for your targeted operating system. Just follow the instructions on the tools' sites.

### Build

The build can be triggered like any other `cmake` project.

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

### Tests

The tests for the native library are using [catch2](https://github.com/catchorg/Catch2) (provided in *tests/catch2*) and should be automatically picked up by [ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html).

The `Python` library uses [pytest](https://docs.pytest.org) for its tests and a adds custom target `foo-python-tests` to `ctest`.

```sh
cmake --build .
ctest .
```

### Documentation

For further information on the API you can build additional documentation yourself. Use the `BUILD_DOCUMENTATION` flag when configuring `cmake` to add the custom target `documentation` to the build (also added to the `ALL` build).

If enabled the native API documentation will be built using [doxygen](https://www.doxygen.nl/) and the Python documentation will be built using [sphinx](https://www.sphinx-doc.org/).

```sh
cmake -DBUILD_DOCUMENTATION=ON ..
cmake --build . --target documentation
```

The native documentation will be put in a directory *docs* in `cmake`'s build directory. The documentation for the Python bindings will be put in *extras/python-bindings/build/sphinx*.

### Installation

The repository is configured to be usable as either a `git submodule` or to be installable like any other `cmake` project.

#### Use as Git Submodule

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

#### Install to be Usable Outside of CMake

If you want to install the native library and the Python bindings you can also use the `cmake install` target (might require *superuser* privileges):

```sh
cmake --build .
sudo cmake --build . --target install
```

This will install:
* The native `foo` library (to somewhere like */usr/local/lib/*)
* The required headers for the `foo` library (to somewhere like */usr/local/include/*)
* The cmake files for `find_package` (to somewhere like */usr/local/share/cmake/*)
* The Python `foo` package (to somewhere like */usr/local/lib/python3.6/dist-packages/*)
* If enabled both the native documentation as well as the Python documentation (to somewhere like */usr/local/share/doc/*)
