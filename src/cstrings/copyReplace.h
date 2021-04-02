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
 * @file copyReplace.h
 *
 * Functions that copy c-strings from `src` to `dst` while replacing a
 * given character with another character or another string.
 */

#ifndef ACE_COMMON_COPY_REPLACE_H
#define ACE_COMMON_COPY_REPLACE_H

#include <stddef.h> // size_t

namespace ace_common {

/**
  * Copy at most dstSize characters from src to dst, while replacing all
  * occurrences of oldChar with newChar. If newChar is '\0', then replace with
  * nothing. The resulting dst string is always NUL terminated.
  */
void copyReplaceChar(char* dst, size_t dstSize, const char* src,
    char oldChar, char newChar);

/**
  * Copy at most dstSize characters from src to dst, while replacing all
  * occurrence of oldChar with newString. If newString is "", then replace
  * with nothing. The resulting dst string is always NUL terminated.
  */
void copyReplaceString(char* dst, size_t dstSize, const char* src,
    char oldChar, const char* newString);

} // ace_common

#endif
