/*
MIT License

Copyright (c) 2020 Brian T. Park

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

/**
 * @file djb2.h
 *
 * Implement the djb2 hash algorithm (http://www.cse.yorku.ca/~oz/hash.html)
 * for normal strings and flash strings.
 */

#ifndef ACE_COMMON_HASH_DJB2_H
#define ACE_COMMON_HASH_DJB2_H

#include <stdint.h> // uint32_t
#include "../fstrings/FlashString.h"
class __FlashStringHelper;

namespace ace_common {

/**
 * Implement the djb2 hash algorithm as described in
 * https://stackoverflow.com/questions/7666509 and
 * http://www.cse.yorku.ca/~oz/hash.html.
 *
 * @tparam T pointer type of the string, either (const char*) or (const
 * __FlashStringHelper*)
 *
 * @param s NUL terminated string, cannot be nullptr
 */
template <typename T>
uint32_t hashDjb2(T s) {
  uint32_t hash = 5381;
  uint8_t c;

  while ((c = *s++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}

/**
 * Specialization of hashDjb2(T s) for flash strings (const
 * __FlashStringHelper*).
 *
 * @param fs NUL terminated string stored in Flash memory, cannot be nullptr
 */
template <>
inline uint32_t hashDjb2(const __FlashStringHelper* fs) {
  return hashDjb2<FlashString>(FlashString(fs));
}

} // ace_common

#endif
