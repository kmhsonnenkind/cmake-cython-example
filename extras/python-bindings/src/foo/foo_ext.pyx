# MIT License
#
# Copyright (c) 2020 Martin Kloesch
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

'''
Actual implementation for wrapper class `Foo` wrapping native `Foo` struct.
'''

__all__ = ['Foo']


from libc.errno cimport ENOMEM, EINVAL, ERANGE

from foo cimport c_foo


cdef class Foo:
    def __cinit__(self, int baz=0):
        '''
        Constructor initializes native struct.

        :param baz: Initial value for the Foo's baz.
        :type baz: int
        '''
        cdef int result = c_foo.foo_initialize(&self._native, baz)

        if result == EINVAL:
            raise ValueError("parameter 'baz' must not be negative")
        elif result == ENOMEM:
            raise MemoryError()

    def __dealloc__(self):
        '''
        Destructor calls native cleanup function.
        '''
        c_foo.foo_destroy(&self._native)

    def foob(self):
        '''
        Foob's the Foo's baz by calling native function.

        :raise OverflowError: If foob'ing the would overflow the baz.
        '''
        cdef int result = c_foo.foo_foob(&self._native)

        if result == ERANGE:
            raise OverflowError("could not foob the Foo's baz")
        elif result == EINVAL:
            # Should not occur
            raise RuntimeError("error occurred in native library")

    @property
    def baz(self):
        '''
        :return: The current value of the Foo's baz.
        :rtype: int
        '''
        cdef int baz = 0
        cdef int result = c_foo.foo_get_baz(&self._native, &baz)

        if result == EINVAL:
            # Should not occur
            raise RuntimeError("error occurred in native library")

        return baz
