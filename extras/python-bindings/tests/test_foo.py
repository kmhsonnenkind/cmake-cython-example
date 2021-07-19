# MIT License
#
# Copyright (c) 2021 Martin Kloesch
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
Test cases for wrapper to native `Foo` struct.
'''

import pytest

import foo


#: Maximum value for signed integer in cython
CYTHON_INT_MAX: int = 0x7fffffff


def test_foo_init_no_param():
    '''
    Tests that default constructor initializes members with expected
    values.
    '''
    instance = foo.Foo()
    assert instance.baz == 0


def test_foo_init():
    '''
    Tests that constructor correctly sets members.
    '''
    instance = foo.Foo(42)
    assert instance.baz == 42


def test_foo_init_bar_negative():
    '''
    Tests that constructor raises exception when parameter `baz` is negative.
    '''
    with pytest.raises(ValueError):
        foo.Foo(-1)


def test_foo_foob():
    '''
    Tests that :meth:`Foo.foob()` actually foob's the Foo's baz.
    '''
    instance = foo.Foo(42)
    assert instance.baz == 42

    instance.foob()
    assert instance.baz == 43

    instance.foob()
    assert instance.baz == 44


def test_foo_foob_overflow():
    '''
    Tests that :meth:`Foo.foob()` raises exception when baz would overlow
    '''
    instance = foo.Foo(CYTHON_INT_MAX - 1)
    assert instance.baz == (CYTHON_INT_MAX - 1)

    instance.foob()
    assert instance.baz == CYTHON_INT_MAX

    with pytest.raises(OverflowError):
        instance.foob()

    assert instance.baz == CYTHON_INT_MAX
