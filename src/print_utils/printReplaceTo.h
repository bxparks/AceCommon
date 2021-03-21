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

/**
 * @file printReplaceTo.h
 *
 * Functions that print strings from `src` to the given `Print` object while
 * replacing a given character with another character or another string.
 */

#ifndef ACE_COMMON_PRINT_REPLACE_TO_H
#define ACE_COMMON_PRINT_REPLACE_TO_H

#include <stddef.h> // size_t
#include <Print.h> // Print
#include "../fstrings/FlashString.h"

class Print;
class __FlashStringHelper;

namespace ace_common {

/**
 * Print the src to printer while replacing all occurrences of oldChar with
 * newChar. If newChar is '\0', then replace with nothing.
 *
 * @tparam T type that acts like a (const char*)
 */
template <typename T>
void printReplaceCharTo(Print& printer, T src, char oldChar, char newChar) {
  char c;
  while ((c = *src++) != '\0') {
    if (c == oldChar) {
      if (newChar == '\0') continue;
      c = newChar;
    }
    printer.write(c);
  }
}

/**
 * Version of printReplaceCharTo() that works for a (const __FlashStringHelper*)
 * by wrapping a FlashString around it.
 */
// 'inline' required, see https://stackoverflow.com/questions/4445654
template<>
inline void printReplaceCharTo<const __FlashStringHelper*>(
    Print& printer, const __FlashStringHelper* src,
    char oldChar, char newChar) {
  printReplaceCharTo<FlashString>(printer, FlashString(src), oldChar, newChar);
}

/**
 * Print the src to print while replacing all occurrence of oldChar with
 * newString. If newString is "", then replace with nothing.
 *
 * @tparam T cstring-like type, a (const char*) or a FlashString
 */
template <typename T>
void printReplaceStringTo(
    Print& printer, T src, char oldChar, const char* newString) {
  char c;
  while ((c = *src++) != '\0') {
    if (c == oldChar) {
      printer.print(newString);
    } else {
      printer.write(c);
    }
  }
}

/**
 * Version of printReplaceCharTo() that works for a (const __FlashStringHelper*)
 * by wrapping a FlashString around it.
 */
// 'inline' required, see https://stackoverflow.com/questions/4445654
template<>
inline void printReplaceStringTo<const __FlashStringHelper*>(
    Print& printer, const __FlashStringHelper* src,
    char oldChar, const char* newString) {
  printReplaceStringTo<FlashString>(
      printer, FlashString(src), oldChar, newString);
}

} // ace_common

#endif
