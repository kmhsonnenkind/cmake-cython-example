## Example: Implement C API using Cython

This subdirectory contains all files required to implement the [example C API](../../include/foo.h) using [cython](https://cython.org).

The `cython` implementation file [foo_impl.pyx](foo_impl.pyx) contains all required function implementations of the example `foo` library. As with all `cython` code, it can take `C-style` in- and output parameters but also has access to all the nice `Python` functionality on top. This means you can wrap your Python libraries for direct access in native code and avoid complicated ports or native rewrites.

[foo_impl.cpp](foo_impl.cpp) is used to transparently load the library without the need of any further modifications of your main application code. With the combination of the `cython` implementation and the patched loading mechanism, you can swap out native implementation with the `Python` based one without too many complications. If you have a look at the [test code](../../tests/foo-tests.cpp), no extra configuration is required and the tests work transparently by simply linking this implementation instead of the [native](../native/foo_impl.c) one.

To see the required build steps you can have a look at the main [CMakeLists.txt](../../CMakeLists.txt) file. The build basically boils down to:

* Using `cython` to generate the wrapper code (`foo_impl.c` and `foo_impl.h`).
* Build the library definition using the wrapper code as well as the [patched loader](foo_impl.cpp) (requires `Python.h` at compile time).
* Link the libray with the matching `Python` library (ala `-lpython3.10`)
