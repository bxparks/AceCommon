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

#include <Print.h>
#include "url_encoding.h"

namespace ace_common {

/**
 * Convert a character into percent-encoded %{hex} format in the form of
 * %{{code0}{code1}}.
 *
 * I tried returning the (code0, code1) pair as a uint16_t type, then breaking
 * apart the 2 bytes in the calling routine. But that alternative was far
 * slower than using a reference to code0 and code1. I suspect that the
 * compiler is able to optimize away the reference and possibly inline the
 * entire function into the calling code.
 */
static void intToHex(char c, char& code0, char& code1) {
  char c1 = (c & 0xf);
  char c0 = (c >> 4) & 0xf;
  code1 = (c1 > 9) ? c1 - 10 + 'A' : c1 + '0';
  code0 = (c0 > 9) ? c0 - 10 + 'A' : c0 + '0';
}

void formUrlEncode(Print& output, const char* str) {
  while (true) {
    char c = *str;
    if (c == '\0') break;

    if (c == ' ') {
      output.print('+');
    } else if (isalnum(c)) {
      output.print(c);
    } else {
      char code0;
      char code1;
      intToHex(c, code0, code1);
      output.print('%');
      output.print(code0);
      output.print(code1);
    }

    str++;
  }
}

/** Convert a hex character [0-9a-fA-F] into its integer value. */
static char hexToInt(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }

  // Handle 'A'-'F' before 'a'-'f' since percent encoding with capital letters
  // are expected to be the norm.
  if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  }
  return 0;
}

void formUrlDecode(Print& output, const char* str) {
  while (true) {
    char c = *str;
    str++;
    if (c == '\0') break;

    if (c == '+') {
      c = ' ';
    } else if (c == '%') {
      // Convert %{hex} to character
      char code0 = *str;
      str++;
      if (code0 == '\0') break;

      char code1 = *str;
      str++;
      if (code1 == '\0') break;

      c = (hexToInt(code0) << 4) | hexToInt(code1);
    }
    output.print(c);
  }
}

}
