/*
 * MIT License
 *
 * Copyright (c) 2022 Martin Kloesch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file foo_impl.cpp
 * \author Martin Kloesch <martin@gmail.com>
 * \brief Utility wrapper making Python library for foob'ing the Foo's baz
 *        available in native code.
 */

#include <iostream>
#include "foo.h"
#include "foo_impl.h"

/**
 * \brief Hook for library loader, importing cython module and making it
 *        available for native code.
 */
__attribute__((constructor)) void __load(void)
{
    if (PyImport_AppendInittab("foo_impl", PyInit_foo_impl) != 0)
    {
        throw std::runtime_error(
            "internal error while instantiating Python based library");
    }

    Py_Initialize();

    if (PyImport_ImportModule("foo_impl") == NULL)
    {
        Py_Finalize();
        throw std::runtime_error(
            "internal error while instantiating Python based library");
    }
}

/**
 * \brief Hook for library loader, unloading cython module and releasing
 *        resources.
 */
__attribute__((destructor)) void __unload(void)
{
    Py_Finalize();
}
