/*
MIT License

Copyright (c) 2021 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ACE_COMMON_FLASH_STRING_H
#define ACE_COMMON_FLASH_STRING_H

#include <Arduino.h>

namespace ace_common {

/**
 * A thin wrapper around a (const __FlashStringHelper*) so that it acts exactly
 * like a (const char*) with regards to operator*(), operator++() and other
 * common operations on (const char*).
 *
 * This is intended to allow template functions to be written to avoid the need
 * to write almost duplicate code twice, one for (const char*) and another for
 * (const __FlashStringHelper*) with the dereferencing replaced by a
 * `pgm_read_byte()` function.
 *
 * Instead, write the templatized code once for a generic `T` type that acts
 * like a `const char*`. That template will be used directly when called with an
 * actual `const char*`. Then create a template specialization for (const
 * __FlashStringHelper*) whose implementation just wraps the `const
 * __FlashStringHelper*` pointer inside a FlashString, then forwards the call to
 * the templatized function with a FlashString instead.
 */
class FlashString {
  public:
    /** Constructor, with implicit conversion. */
    FlashString(const __FlashStringHelper* p)
        : fsp((const char*) p) {}

    /** Default copy constructor. */
    FlashString(const FlashString&) = default;

    /** Default assignment operator. */
    FlashString& operator=(const FlashString&) = default;

    /** Dereference operator. */
    char operator*() const { return (char) pgm_read_byte(fsp); }

    /** Array dereference operator. */
    char operator[](size_t i) const { return *(fsp + i); }

    /**
     * Implicit cast to `const void*` to support operator==() and operator!=().
     *
     * I tried using an implicit `operator const __FlashStringHelper*() const`
     * here. But when I try use `operator[]` on the object, some of the C++
     * compilers (not AVR) produce a warning that says: "ISO C++ says that these
     * are ambiguous, even though the worst conversion for the first is better
     * than the worst conversion for the second". (Other compilers don't produce
     * this error.) As far as I can tell, the compiler thinks that instead of
     * just calling the `operator[]` that I provide above, it can do an implicit
     * conversion to `const __FlashStringHelper*`, then call the built-in
     * `operator[]` on *that* pointer, and it thinks that the 2 paths are
     * equally valid. This is above my understanding of C++ implicit conversion
     * rules.
     *
     * My workaround is to create this implicit cast operator to `const void*`,
     * so that I can get `operator==()` and `operator!=()` to work as expected.
     * Then create an explicit `operator const __FlashStringHelper*()` so that I
     * can convert a FlashString back to its original pointer when I need to.
     */
    operator const void*() const {
      return (const void*) fsp;
    }

    /** Retrieve the original pointer with an explicit cast. */
    explicit operator const __FlashStringHelper*() const {
      return (const __FlashStringHelper*) fsp;
    }

    /** Pre-increment. */
    FlashString operator++() {
      return (const __FlashStringHelper*) ++fsp;
    }

    /** Post-increment. */
    FlashString operator++(int) {
      return (const __FlashStringHelper*) fsp++;
    }

    /** Pre-decrement. */
    FlashString operator--() {
      return (const __FlashStringHelper*) --fsp;
    }

    /** Post-decrement. */
    FlashString operator--(int) {
      return (const __FlashStringHelper*) fsp--;
    }

  private:

    const char* fsp;
};

} // ace_common

#endif
