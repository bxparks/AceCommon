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

/**
 * @file arithmetic.h
 *
 * Common functions involving arithmetics such as incrementing, addition,
 * subtraction, division, and conversion to and from BCD (binary coded
 * decimal).
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

/**
 * A version of decToBcd() that uses only a single division operation. For most
 * 32-bit processors, this version is twice as fast as decToBcdDivMod().
 */
inline uint8_t decToBcdDivOnly(uint8_t val) {
  uint8_t tens = val / 10;
  uint8_t ones = val - tens * 10;
  return (tens * 16) + ones;
}

/**
 * A version of decToBcd() that uses a division and a modulus operation.
 *
 * Some compilers (e.g. AVR) seem to recognize this pattern of
 * division-and-modulus operations, and generate code that is actually faster
 * for this than decToBcdDivOnly(). AutoBenchmark says this version takes 4.996
 * micros, versus 5.128 micros for decToBcdDivOnly() on a AVR. That appears to
 * be the difference of 2 clock cycles for a 16MHz clock.
 */
inline uint8_t decToBcdDivMod(uint8_t val) {
  return (val / 10 * 16) + (val % 10);
}

/**
 * Convert normal decimal numbers to binary coded decimal.
 *
 * Uses either decToBcdDivOnly() or decToBcdDivMod() depending on which was
 * determined to be faster on a particular platform.
 *
 * If the input `val` is greater than 99, the result is unspecified, but is
 * probably not what you want.
 */
inline uint8_t decToBcd(uint8_t val) {
#if defined(ARDUINO_ARCH_AVR)
  return decToBcdDivMod(val);
#else
  return decToBcdDivOnly(val);
#endif
}

/**
 * Convert binary coded decimal to normal decimal numbers.
 *
 * If the input `val` is a valid decimal number (0x00 to 0x99, using only
 * decimal digits), the result is unspecified but probably not what you want.
 */
inline uint8_t bcdToDec(uint8_t val) {
  return (val / 16 * 10) + (val % 16);
}

/**
 * Approximate division by 1000 without using integer division to avoid
 * inefficient integer division operations on 8-bit processors. This can be
 * useful for converting millis to seconds, or micros to millis, for example.
 *
 * The algorithm is based on the binomial expansion of 1/(1-x):
 *
 * @code
 * 1/1000 = 1/(1024 - 24)
 *        = (1/2^10) * (1 / (1 - 3/2^7))
 *        = (1/2^10) * (1 + 3/2^7 + 9/2^14 + 27/2^21 + ...)
 *        = (1/2^10 + 3/2^17 + 9/2^24 + 27/2^31 + ...)
 * @endcode
 *
 * The truncation of this series means that this function returns
 * approximations which are always slightly smaller than the fully accurate
 * answer. See the unit tests for concrete examples. This approximation was
 * good enough for me in the situations where exact accuracy was not required.
 *
 * More accurate algorithms exist (see for example
 * http://www.hackersdelight.org/divcMore.pdf).
 */
inline unsigned long udiv1000(unsigned long n) {
  unsigned long x = (n >> 8);
  unsigned long y = (x >> 8);
  unsigned long z = (y >> 8);
  return (x >> 2) + 3 * (y >> 1) + 9 * z;
}

}

#endif
