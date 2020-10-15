/*
 * MIT License
 *
 * Copyright (c) 2020 Martin Kloesch
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
 * \file foo-tests.cpp
 * \author Martin Kloesch <martin@gmail.com>
 * \brief Tests for foob'ing the Foo's baz.
 */
#include "catch2/catch.hpp"
#include <errno.h>
#include <limits.h>
#include "foo.h"

/**
 * \test Tests that foo_initialize() correctly sets Foo's baz.
 */
TEST_CASE("foo_initialize()", "[foo_initialize]")
{
    Foo foo = {NULL};
    REQUIRE(foo_initialize(&foo, 42) == 0);

    int baz = 0;
    REQUIRE(foo_get_baz(&foo, &baz) == 0);
    REQUIRE(baz == 42);

    foo_destroy(&foo);
}

/**
 * \test Tests that foo_initialize() correctly detects illegal \c NULL argument.
 */
TEST_CASE("foo_initialize(): self NULL", "[foo_initialize, error]")
{
    REQUIRE(foo_initialize(NULL, 42) == EINVAL);
}

/**
 * \test Tests that foo_initialize() correctly detects illegal negative initial
 * baz value.
 */
TEST_CASE("foo_initialize(): baz negative", "[foo_initialize, error]")
{
    Foo foo = {NULL};
    REQUIRE(foo_initialize(&foo, -1) == EINVAL);
    REQUIRE(foo._baz == NULL);
}

/**
 * \test Tests that foo_get_baz() correctly returns Foo's baz.
 */
TEST_CASE("foo_get_baz()", "[foo_get_baz]")
{
    Foo foo;
    foo_initialize(&foo, 42);

    int baz = 0;
    REQUIRE(foo_get_baz(&foo, &baz) == 0);
    REQUIRE(baz == 42);

    foo_destroy(&foo);
}

/**
 * \test Tests that foo_get_baz() correctly detects illegal \c NULL argument.
 */
TEST_CASE("foo_get_baz(): self NULL", "[foo_get_baz, error]")
{
    int baz = 0;
    REQUIRE(foo_get_baz(NULL, &baz) == EINVAL);
}

/**
 * \test Tests that foo_get_baz() correctly detects illegal \c NULL argument.
 */
TEST_CASE("foo_get_baz(): baz_buffer NULL", "[foo_get_baz, error]")
{
    Foo foo;
    REQUIRE(foo_initialize(&foo, 0) == 0);

    REQUIRE(foo_get_baz(&foo, NULL) == EINVAL);

    foo_destroy(&foo);
}

/**
 * \test Tests that foo_get_baz() correctly detects illegal uninitialized
 * argument.
 */
TEST_CASE("foo_get_baz(): self not initialized", "[foo_get_baz, error]")
{
    Foo foo = {NULL};
    int baz = 0;
    REQUIRE(foo_get_baz(&foo, &baz) == EINVAL);
}

/**
 * \test Tests that foo_foob() correctly foob's the Foo's baz.
 */
TEST_CASE("foo_foob()", "[foo_foob]")
{
    Foo foo;
    REQUIRE(foo_initialize(&foo, 42) == 0);

    REQUIRE(foo_foob(&foo) == 0);
    int baz = 0;
    REQUIRE(foo_get_baz(&foo, &baz) == 0);
    REQUIRE(baz == 43);

    REQUIRE(foo_foob(&foo) == 0);
    REQUIRE(foo_get_baz(&foo, &baz) == 0);
    REQUIRE(baz == 44);

    foo_destroy(&foo);
}

/**
 * \test Tests that foo_foob() correctly detects illegal \c NULL argument.
 */
TEST_CASE("foo_foob(): self NULL", "[foo_foob, error]")
{
    REQUIRE(foo_foob(NULL) == EINVAL);
}

/**
 * \test Tests that foo_foob() correctly detects illegal uninitialized
 * argument.
 */
TEST_CASE("foo_foob(): self not initialized", "[foo_foob, error]")
{
    Foo foo = {NULL};

    REQUIRE(foo_foob(&foo) == EINVAL);
}

/**
 * \test Tests that foo_foob() correctly detects int overflows.
 */
TEST_CASE("foo_foob(): integer overflow", "[foo_foob, error]")
{
    Foo foo;
    REQUIRE(foo_initialize(&foo, INT_MAX) == 0);

    REQUIRE(foo_foob(&foo) == ERANGE);

    int baz = 0;
    REQUIRE(foo_get_baz(&foo, &baz) == 0);
    REQUIRE(baz == INT_MAX);

    foo_destroy(&foo);
}

/**
 * \test Tests that foo_destroy() correctly cleans up Foo.
 */
TEST_CASE("foo_destroy()", "[foo_destroy]")
{
    Foo foo;
    REQUIRE(foo_initialize(&foo, 42) == 0);

    int baz = 0;
    REQUIRE(foo_get_baz(&foo, &baz) == 0);
    REQUIRE(baz == 42);

    REQUIRE(foo_destroy(&foo) == 0);

    REQUIRE(foo_get_baz(&foo, &baz) == EINVAL);
}

/**
 * \test Tests that foo_destroy() can handle uninitialized Foo objects.
 */
TEST_CASE("foo_destroy(): self not initialized", "[foo_destroy]")
{
    Foo foo = {NULL};

    REQUIRE(foo_destroy(&foo) == 0);

    int baz = 0;
    REQUIRE(foo_get_baz(&foo, &baz) == EINVAL);
}

/**
 * \test Tests that foo_destroy() correctly detects illegal \c NULL argument.
 */
TEST_CASE("foo_destroy(): self NULL", "[foo_destroy, error]")
{
    REQUIRE(foo_destroy(NULL) == EINVAL);
}
