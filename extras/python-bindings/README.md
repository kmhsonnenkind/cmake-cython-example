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

The build uses [cython](https://cython.org) along with [setuptools](https://setuptools.readthedocs.io/en/latest) and requires `Python >= 3.7`.

It expects the [foo C API](https://github.com/kmhsonnenkind/cmake-cython-example) to be installed and findable by the `Python` interpreter.

```
python3 setup.py build_ext
python3 setup.py install
```

To simplify the build process the library is included in the `cmake` targets for the [foo project](https://github.com/kmhsonnenkind/cmake-cython-example).

Follow the build steps in the mentioned repository for an easy way to get the library and all its dependencies to build and run.

### Documentation

If [sphinx](https://www.sphinx-doc.org/) is installed a custom build target `build_sphinx` is added to *setup.py* that will build the HTML based API documentation. It requires the built `foo` package to be available so it will need to build a binary version beforehand (e.g. using the `bdist_egg` target).

```sh
python3 setup.py bdist_egg build_sphinx
```


## Tests

The Python bindings are tested using [pytest](https://docs.pytest.org/). They can be found in the *tests* directory and triggered using *setup.py*'s `test` target.

```sh
python3 setup.py test
```
