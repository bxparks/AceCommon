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
      byteToHexChar(c, &code0, &code1);
      output.print('%');
      output.print(code0);
      output.print(code1);
    }

    str++;
  }
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

      c = (hexCharToByte(code0) << 4) | hexCharToByte(code1);
    }
    output.print(c);
  }
}

void byteToHexChar(uint8_t c, char* high, char* low, char baseChar) {
  char lowNibble = (c & 0xf);
  char highNibble = (c >> 4) & 0xf;
  *low = (lowNibble > 9) ? lowNibble - 10 + baseChar : lowNibble + '0';
  *high = (highNibble > 9) ? highNibble - 10 + baseChar : highNibble + '0';
}

uint8_t hexCharToByte(char c) {
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

}
