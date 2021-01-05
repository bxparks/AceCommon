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

#ifndef ACE_COMMON_KSTRING_H
#define ACE_COMMON_KSTRING_H

#include <stdint.h> // int8_t
class __FlashStringHelper;
class Print;

namespace ace_common {

/**
 * A wrapper class around a normal c-string or Arduino f-string which is
 * encoded and compressed using keyword substitution. The keywords are encoded
 * as non-printable ASCII characters between 0x01 and 0x1f (all below the space
 * character 0x20). The 0x00 character cannot be used since it is the NUL
 * character that terminates the string. The list of keywords are given in the
 * `keywords` array.
 *
 * The number of keywords is obviously limited to 31, which happens to be
 * sufficient for my needs. Instead of using the 31 unprintable characters, we
 * could use the high bit characters (0x80 - 0xff) to get 128 keywords, but I
 * don't need that right now.
 *
 * @param s NUL terminated string, cannot be nullptr
 * @param keywords an array of keywords, with nullptr in index position 0
 * @param numKeywords number of keywords, including the nullptr in the 0th pos
 */
class KString {
  public:
    explicit KString(
      const char* s,
      const char* const* keywords,
      uint8_t numKeywords
    ):
      cstring_(s),
      keywords_(keywords),
      numKeywords_(numKeywords)
    {}

    /**
     * Compare this string against s and return `<0`, `0` or `>0` if this
     * string is less than, equal to, or greater than the target string `s`.
     */
    int compareTo(const char* s);

    /** Expand and print the current string to the given printer. */
    void printTo(Print& printer);

  private:
    const char* const cstring_;
    const char* const* const keywords_;
    uint8_t const numKeywords_;
};

} // ace_common

#endif // ACE_COMMON_HASH_DJB2_H
