#cython: language_level=3

# MIT License
#
# Copyright (c) 2022 Martin Kloesch
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

"""
Cython definitions matching `foo.h` native header.
"""

__all__ = ["foo_foob"]


from libc cimport errno
from libc cimport limits
from libc cimport stdlib


#: External C definitions from foo.h
cdef extern from "foo.h":
    ctypedef struct Foo:
        int *_baz


cdef public int foo_initialize(Foo *self, int baz):
    """
    Initializes all members in Foo struct.

    :param self: Foo object to be initialized.
    :param baz: Initial value for Foo's baz.
    :return: 0 if successful.
             `errno.EINVAL` if `self` is `NULL` or `baz` is negative.
             `errno.ENOMEM` if not enough memory available.
    """
    # Validate parameters
    if self is NULL or baz < 0:
        return errno.EINVAL

    # Dynamically allocate members
    self._baz = <int*> stdlib.malloc(sizeof(int))
    if self._baz is NULL:
        return errno.ENOMEM

    self._baz[0] = baz
    return 0


cdef public int foo_foob(Foo *self):
    """
    Foob's the Foo's baz.

    :param self: Foo object to be initialized.
    :return: 0 if successful.
             `errno.EINVAL` if `self` is `NULL` or uninitialized.
             `errno.ERANGE` if foob'd baz would cause integer overflow.
    """
    # Validate parameters
    if self is NULL or self._baz is NULL:
        return errno.EINVAL

    # Check that no overflow would occur
    if self._baz[0] == limits.INT_MAX:
        return errno.ERANGE

    # Actually foob the baz
    self._baz[0] = self._baz[0] + 1
    return 0


cdef public int foo_get_baz(Foo *self, int *baz_buffer):
    """
    Getter for Foo's baz.

    :param self: Foo object to get baz for.
    :param baz_buffer: Buffer to store baz in.
    :return: 0 if successful.
             `errno.EINVAL` if `self` is `NULL` or uninitialized or `baz_buffer` is `NULL`.
    """
    # Validate parameters
    if self is NULL or  self._baz is NULL or baz_buffer is NULL:
        return errno.EINVAL

    # Save value in out parameters
    baz_buffer[0] = self._baz[0]
    return 0


cdef public int foo_destroy(Foo *self):
    """
    Frees resources associated with Foo object (but not the object itself).

    :param self: Foo object to be destroyed.
    :return: 0 if successful.
             `errno.EINVAL` if `self` is `NULL`.
    """
    # Validate parameters
    if self is NULL:
        return errno.EINVAL

    # Free memory
    if self._baz is not NULL:
        stdlib.free(self._baz)
    self._baz = NULL

    return 0
