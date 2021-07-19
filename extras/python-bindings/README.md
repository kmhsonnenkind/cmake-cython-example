# Python Wrapper for native `foo` Library

This library offers [Python 3](https://www.python.org) bindings for the [foo C API](https://github.com/kmhsonnenkind/cmake-cython-example).


## Example

```python
import foo

# This will also instantiate the native struct
instance = foo.Foo()

# This will wrap to the native library functions
instance.foob()
print(f"The foo's baz is {instance.baz}")
```


## Build and Installation

The build uses [cython](https://cython.org) along with [setuptools](https://setuptools.readthedocs.io/en/latest) and requires `Python >= 3.6`.

It expects the [foo C API](https://github.com/kmhsonnenkind/cmake-cython-example) to be installed and findable by the `Python` interpreter.

```
python3 setup.py build_ext
python3 setup.py install
```

To simplify the build process the library is included in the `cmake` targets for the [foo project](https://github.com/kmhsonnenkind/cmake-cython-example).

Follow the build steps in the mentioned repository for an easy way to get the library and all its dependencies to build and run.

### Documentation

If [sphinx](https://www.sphinx-doc.org/) is installed a custom build target `build_sphinx` is added to *setup.py* that will build the HTML based API documentation. It requires the built `foo` package to be available so it will need to build a binary version beforehand (e.g. using the `build_ext` target).

```sh
python3 setup.py build_ext build_sphinx
```


## Tests

The Python bindings are tested using [pytest](https://docs.pytest.org/). They can be found in the *tests* directory and triggered using *setup.py*'s `pytest` target.

```sh
python3 setup.py pytest
```

### Coverage

It is possible to measure code coverage using [coverage.py](https://coverage.readthedocs.io/en/coverage-5.5/). To do so you need to define the `CYTHON_TRACE` macro when building the extension (`build_ext` step). If you are using the `cmake` build of the main `foo` library you can enable this by setting the `CODE_COVERAGE` flag. This will add the corresponding entry to `setup.cfg`. Otherwise you can also set it from the command line.

```sh
python3 setup.py build_ext --define=CYTHON_TRACE
```

After this execute the tests using the `coverage` module to create the corresponding *.coverage* file with the required trace information. If you are using the `cmake` build, calling `ctest` will also call the corresponding command.

```sh
python3 -m coverage run setup.py pytest
```

#### Cython.Coverage Problem

The cython coverage plugin currently has an issue where it does not report files in cython packages correctly. To work around this problem a utility script *util/coverage-report.py* is provided that monkey-patches the plugin and generates an XML report for you. The script was tested using `coverage.py` version 5.5 and `cython` version 0.29.23 but should work with different versions as well.

To generate the report simply execute the script after having measured the coverage (*.coverage* file present). This will generate an XML report *coverage.xml* in the current working directory.

```sh
python3 util/coverage-report.py
```

You can also have a look at the script to see how to patch `coverage.py` to handle files in `cython` packages and create different reports.
