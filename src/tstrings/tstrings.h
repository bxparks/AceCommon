/*
MIT License

Copyright (c) 2023 Brian T. Park

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
 * @file tstrings.h
 *
 * This file constains selected overloaded functions which act as a thin layer
 * of indirection to their equivalent functions in the `<strings.h>` C library.
 * The overloaded functions take either a `const char*` or a `const
 * __FlashStringHelper*`. This allows template code to be written which is
 * agnostic to whether the source string is in regular memory or flash memory.
 * The compiler will select the correct implementation automatically at
 * compile-time.
 *
 * Ideally all functions included in `<pgmspace.h>` that end in the `_P` suffix
 * would be included here. But I don't have infinite time, so this currently
 * includes only the most popular ones, or the ones that are needed for my own
 * libraries. Additional functions can be added incrementally.
 */

#ifndef ACE_COMMON_TSTRINGS_H
#define ACE_COMMON_TSTRINGS_H

#include <string.h>
#include <Arduino.h> // xxx_P()

class __FlashStringHelper;

namespace ace_common {

// Functions are ordered alphabetically for ease of maintenance.

inline char* strcat_T(char* dest, const char* src) {
  return strcat(dest, src);
}
inline char* strcat_T(char* dest, const __FlashStringHelper* src) {
  return strcat_P(dest, (const char*) src);
}

inline const char* strchr_T(const char* s, int c) {
  return strchr(s, c);
}
inline const char* strchr_T(const __FlashStringHelper* s, int c) {
  return strchr_P((const char*) s, c);
}

inline int strcmp_T(const char* s1, const char* s2) {
  return strcmp(s1, s2);
}
inline int strcmp_T(const char* s1, const __FlashStringHelper* s2) {
  return strcmp_P(s1, (const char*) s2);
}

inline char* strcpy_T(char* dest, const char* src) {
  return strcpy(dest, src);
}
inline char* strcpy_T(char* dest, const __FlashStringHelper* src) {
  return strcpy_P(dest, (const char*) src);
}

inline size_t strlen_T(const char* s) {
  return strlen(s);
}
inline size_t strlen_T(const __FlashStringHelper* s) {
  return strlen_P((const char*) s);
}

inline char* strncat_T(char* dest, const char* src, size_t n) {
  return strncat(dest, src, n);
}
inline char* strncat_T(char* dest, const __FlashStringHelper* src, size_t n) {
  return strncat_P(dest, (const char*) src, n);
}

inline int strncmp_T(const char* s1, const char* s2, size_t n) {
  return strncmp(s1, s2, n);
}
inline int strncmp_T(const char* s1, const __FlashStringHelper* s2, size_t n) {
  return strncmp_P(s1, (const char*) s2, n);
}

inline char* strncpy_T(char* dest, const char* src, size_t n) {
  return strncpy(dest, src, n);
}
inline char* strncpy_T(char* dest, const __FlashStringHelper* src, size_t n) {
  return strncpy_P(dest, (const char*) src, n);
}

inline const char* strrchr_T(const char* s, int c) {
  return strrchr(s, c);
}
inline const char* strrchr_T(const __FlashStringHelper* s, int c) {
  // ESP8266 and ESP32 don't provide strrch_P(), but AceCommon/pstrings.h does.
  return strrchr_P((const char*) s, c);
}

}

#endif
