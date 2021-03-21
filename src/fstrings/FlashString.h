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

    /** Cast operator. */
    operator const __FlashStringHelper*() const {
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
