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
 * @file pstrings.h
 *
 * String functions on strings stored in flash memory through the PROGMEM
 * attribute. These strings can be created through several ways:
 *
 *    * PROGMEM attribute
 *    * F() macro
 *    * PSTR() macro
 */

#ifndef ACE_COMMON_PSTRINGS_H
#define ACE_COMMON_PSTRINGS_H

namespace ace_common {

/**
 * Compare 2 strings which are both stored in PROGMEM. This is similar to the
 * `strcmp_P(a, p)` except that both strings are in PROGMEM instead of just the
 * second string. Either `a` or `b` can be a `nullptr`. Two `nullptr` are
 * compared to be equal to each other. Any non-null string will be considered to
 * be greater than a `nullptr`.
 */
int strcmp_PP(const char* a, const char* b);

// These PROGMEM string functions exist in AVR but not in ESP8266 or ESP32.
#if defined(ESP8266) || defined(ESP32)

  /**
   * Return a pointer to the first occurence of `c` in the string `s` stored in
   * PROGMEM. Returns nullptr if not found.
   */
  const char* strchr_P(const char* s, int c);

  /**
   * Return a pointer to the last occurence of `c` in the string `s` stored in
   * PROGMEM. Returns nullptr if not found.
   */
  const char* strrchr_P(const char* s, int c);

#endif

}

#endif
