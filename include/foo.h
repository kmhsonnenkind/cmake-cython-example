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
 * \file foo.h
 * \author Martin Kloesch <martin.kloesch@gmail.com>
 * \brief Library for foob'ing the Foo's baz.
 */

#pragma once

#ifndef FOO_H
#define FOO_H

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \struct Foo
 * \brief Foo struct storing the baz.
 */
typedef struct
{
    /**
     * \brief Private member for the Foo's baz.
     *
     * \details Manipulated internally, do **NOT** set manually! Use
     *          foo_get_baz() to get value.
     *
     * \see foo_get_baz()
     */
    int *_baz;
} Foo;

/**
 * \brief Initializes all members in Foo struct.
 *
 * \param[in] self Foo object to be initialized.
 * \param[in] baz Initial value for Foo's baz.
 * \return int \c 0 if successful.
 * \retval 0 success.
 * \retval EINVAL if \p self is \c NULL or \p baz is negative.
 * \retval ENOMEM if not enough memory available.
 * \relates Foo
 */
int foo_initialize(Foo *self, int baz);

/**
 * \brief Foob's the Foo's baz.
 *
 * \param[in] self Foo object whose baz shall be foob'd.
 * \return int \c 0 if successful.
 * \retval 0 success.
 * \retval EINVAL if \p self is \c NULL or uninitialized (foo_initialize()
 *         not called or foo_destroy() called).
 * \retval ERANGE if foob'd baz would cause integer overflow.
 * \relates Foo
 */
int foo_foob(Foo *self);

/**
 * \brief Getter for Foo's baz.
 *
 * \param[in] self Foo object to get baz for.
 * \param[out] baz_buffer Buffer to store baz in.
 * \return int \c 0 if successful.
 * \retval 0 success.
 * \retval EINVAL if \p self is \c NULL or unitintialized or \p baz_buffer
 *         is \c NULL.
 * \relates Foo
 */
int foo_get_baz(Foo *self, int *baz_buffer);

/**
 * \brief Frees resources associated with Foo object (but not the object
 *        itself).
 *
 * \details Foo objects initialized with foo_initialize() might have members
 *          set that need explicit cleanup. Calling this function will ensure
 *          all data has been freed and all locks released.
 *
 * \param[in] self Foo object to be destroyed.
 * \return int \c 0 if successful.
 * \retval 0 success.
 * \retval EINVAL if \p self is \c NULL.
 * \relates Foo
 */
int foo_destroy(Foo *self);

#ifdef __cplusplus
}
#endif

#endif // FOO_H
