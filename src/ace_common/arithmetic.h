/*
MIT License

Copyright (c) 2018, 2020 Brian T. Park

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

#ifndef ACE_COMMON_ARITHMETIC_H
#define ACE_COMMON_ARITHMETIC_H

#include <stdint.h>

namespace ace_common {

/**
 * Increment 'd' mod 'm', avoiding '%' operator which is expensive for 8-bit
 * processors. For example, cycling through 0 to 9 can be done with
 * `incrementMod(count, 10)`.
 */
template<typename T>
void incrementMod(T& d, T m) {
  d++;
  if (d >= m) d = 0;
}

/**
 * Increment 'd' mod 'm', with an offset, avoiding '%' operator which is
 * expensive for 8-bit processors. For example, incrementing the months of the
 * year from 1 to 12 can be done with `incrementMod(month, 12, 1)`.
 */
template<typename T>
void incrementModOffset(T& d, T m, T offset) {
  d -= offset;
  d++;
  if (d >= m) d = 0;
  d += offset;
}

/** Convert normal decimal numbers to binary coded decimal. */
inline uint8_t decToBcd(uint8_t val) {
  return (val / 10 * 16) + (val % 10);
}

/** Convert binary coded decimal to normal decimal numbers. */
inline uint8_t bcdToDec(uint8_t val) {
  return (val / 16 * 10) + (val % 16);
}

}

#endif
