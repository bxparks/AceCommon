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
 * @file url_encoding.h
 *
 * Provides 2 functions to perform URL form encoding and decoding. A space `'
 * '` is converted into a `'+'`, and all other non-alphabetical characters are
 * converted into %{hex}.
 */

#ifndef ACE_COMMON_URL_ENCODING_H
#define ACE_COMMON_URL_ENCODING_H

#include <stdint.h> // uint8_t

class Print;

namespace ace_common {

/**
 * Encode the str using an encoding suitable for GET parameters and forms in
 * the body of a POST that expects a application/x-www-form-urlencoded type. A
 * ' ' is converted into a '+' and non-alphanumerics are percent-encoded.
 *
 * The result is printed to the `output` that implements the `Print` interface.
 * The `output` could be the `Serial` object, but more frequently, it is
 * useful to use an in-memory buffer such as `PrintStr`. This allows us to
 * avoid using a String object, which decreases the risk of heap fragmentation.
 *
 * See https://en.wikipedia.org/wiki/Percent-encoding and
 * https://stackoverflow.com/questions/1634271.
 */
void formUrlEncode(Print& output, const char* str);

/**
 * Decode the str that was encoded using form_url_encode().
 */
void formUrlDecode(Print& output, const char* str);

/**
 * Convert a byte into 2-digit, uppercase, hex. For example, 0x9A returns the
 * '9' and 'A' characters.
 *
 * I tried returning the (high, low) pair as a uint16_t type, then breaking
 * apart the 2 bytes in the calling routine. But that alternative was slower
 * than using a reference to code0 and code1. I suspect that the compiler is
 * able to optimize away the reference and possibly inline the entire function
 * into the calling code.
 *
 * @param c the 8-bit unsigned value to be converted into hexadecimal
 * @param high output parameter of the high character
 * @param low output parameter of the low character
 * @param baseChar the base character of the hexadecimal range, default 'A'
 *   which returns hexadecimal characters using uppercase. Set this to 'a' to
 *   get lowercase characters.
 */
void byteToHexChar(uint8_t c, char* high, char* low, char baseChar = 'A');

/**
 * Convert a hex character [0-9a-fA-F] into its integer value. For example,
 * 9 returns '9', and 10 returns 'A'. Characters outside of the valid
 * hexadecimal character range return 0.
 */
uint8_t hexCharToByte(char c);

}

#endif
