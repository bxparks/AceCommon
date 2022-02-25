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

#include "backslash_x_encoding.h"
#include "../url_encoding/url_encoding.h"

namespace ace_common {

uint8_t backslashXEncode(char* t, size_t tcap, const char* s, size_t* written) {
  char* tt = t;
  char* const tend = t + tcap - 1;

  // The code becomes smaller if we assume an error status as the default.
  uint8_t status = 1;

  while (true) {
    // Check for end of 's', before checking for 'tend'.
    uint8_t c = *s++;
    if (c == '\0') {
      status = 0;
      break;
    }

    // Check for end of 't' before every write to 't'.
    if (tt >= tend) break;

    if (c >= ' ' && c < 127) {
      *tt++ = c;
      if (c == '\\') {
        if (tt >= tend) break;
        *tt++ = '\\';
      }
    } else {
      *tt++ = '\\';

      if (tt >= tend) break;
      *tt++ = 'x';

      char high;
      char low;
      byteToHexChar(c, &high, &low);

      if (tt >= tend) break;
      *tt++ = high;

      if (tt >= tend) break;
      *tt++ = low;
    }
  }

  *tt = '\0';
  if (written) {
    *written = tt - t;
  }
  return status;
}

uint8_t backslashXDecode(char* t, size_t tcap, const char* s, size_t* written) {
  char* tt = t;
  char* const tend = t + tcap - 1;

  // The code becomes smaller if we assume an error status as the default.
  uint8_t status = 1;

  while (true) {
    uint8_t c = *s++;
    if (c == '\0') {
      status = 0;
      break;
    }

    if (tt >= tend) break;

    // Decode the next character.
    if (c == '\\') {
      char escape = *s++;
      // Reaching the end of string in the middle of the encoding is an error.
      if (escape == '\0') break;

      // Convert \\ or \xHH to character
      if (escape == '\\') {
        c = '\\';
      } else {
        if (escape != 'x') break;

        // Verify that "HH" characters are valid hexadecimal.
        char high = *s++;
        if (high == '\0') break;
        if (! isHexChar(high)) break;

        char low = *s++;
        if (low == '\0') break;
        if (! isHexChar(low)) break;

        c = (hexCharToByte(high) << 4) | hexCharToByte(low);
      }
    } else if (c < ' ' || c >= 127) {
      // If source string contains any unprintable character, mark as error.
      break;
    }

    *tt++ = c;
  }

  *tt = '\0';
  if (written) {
    *written = tt - t;
  }
  return status;
}

}
