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
 * \file foo_impl.c
 * \author Martin Kloesch <martin@gmail.com>
 * \brief Foo library foob'ing the Foo's baz.
 */

#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#include "foo.h"

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
int foo_initialize(Foo *self, int baz)
{
    // Validate parameters
    if ((self == NULL) || (baz < 0))
    {
        return EINVAL;
    }

    // Dynamically allocate members
    self->_baz = malloc(sizeof(int));
    if (self->_baz == NULL)
    {
        return ENOMEM; // LCOV_EXCL_LINE
    }
    *self->_baz = baz;
    return 0;
}

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
int foo_foob(Foo *self)
{
    // Validate parameters
    if ((self == NULL) || (self->_baz == NULL))
    {
        return EINVAL;
    }

    // Check that no overflow would occur
    if (*self->_baz == INT_MAX)
    {
        return ERANGE;
    }

    // Actually foob the baz
    *self->_baz += 1;
    return 0;
}

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
int foo_get_baz(Foo *self, int *baz_buffer)
{
    // Validate parameters
    if ((self == NULL) || (self->_baz == NULL) || (baz_buffer == NULL))
    {
        return EINVAL;
    }

    // Save value in out parameters
    *baz_buffer = *self->_baz;
    return 0;
}

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
int foo_destroy(Foo *self)
{
    // Validate parameters
    if (self == NULL)
    {
        return EINVAL;
    }

    // Free memory
    if (self->_baz != NULL)
    {
        free(self->_baz);
    }
    self->_baz = NULL;

    return 0;
}
