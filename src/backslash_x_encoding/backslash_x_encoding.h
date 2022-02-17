/*
MIT License

Copyright (c) 2022 Brian T. Park

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
 * @file backslash_x_encoding.h
 *
 * Provides 2 functions to perform backslash-x encoding and decoding.
 * Non-printable characters are converted into `\xHH` form, where `HH` are
 * hexadecimal digits. A `\` is converted into `\x5A`.
 */

#ifndef ACE_COMMON_BACKSLASH_X_ENCODING_H
#define ACE_COMMON_BACKSLASH_X_ENCODING_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

namespace ace_common {

/**
 * Encode the source 's' to target string 't' with capacity 'tcap'. The target
 * 't' is always NUL-terminated.
 *
 * @param t target c-string
 * @param tcap capacity of t
 * @param s source c-string
 * @param status 0 if successful, non-zero if error
 * @return the number of bytes written to 't'
 */
size_t backslashXEncode(char* t, size_t tcap, const char* s, uint8_t* status);

/**
 * Decode the source 's' to target string 't' with capacity 'tcap'. The target
 * 't' is always NUL-terminated.
 *
 * @param t target c-string
 * @param tcap capacity of t
 * @param s source c-string
 * @param status return 0 upon success; 1 if 't' ran out of capacity; 2 if 's'
 *    was ill-formed (e.g. the `\x` was not followed by 2 hex characters
 * @return the number of bytes written to 't'
 */
size_t backslashXDecode(char* t, size_t tcap, const char* s, uint8_t* status);

/** Return true if 'c' is a hex character [0-9a-fA-F]. */
inline bool isHexChar(char c) {
  return (c >= '0' && c <= '9')
    || (c >= 'a' && c <= 'f')
    || (c >= 'A' && c <= 'F');
}

}

#endif
