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

class KStringIterator;

/**
 * A wrapper class around a normal c-string or Arduino f-string which is
 * encoded and compressed using keyword substitution. The keywords are encoded
 * as non-printable ASCII characters between 0x01 and 0x1f (all below the space
 * character 0x20). The 0x00 character cannot be used since it is the NUL
 * character that terminates the string. The list of keywords are given in the
 * `keywords` array.
 *
 * The number of keywords is obviously limited to 31, which happens to be
 * sufficient for my needs. An alternative implementation is to use the high
 * bit characters (0x80 - 0xff) which gives us 128 keywords, but I don't need
 * that right now.
 *
 * This class is expected to have a short lifetype. In normal usage, it is
 * created on the stack, the compareTo() or printTo() is function called, then
 * it is auto-destroyed at the end of its scope.
 */
class KString {
  friend class KStringIterator;

  public:
    /**
     * Constructor around a simple c-string.
     *
     * @param s NUL terminated string or nullptr
     * @param keywords an array of keywords, up to 32 slots. The first slot at
     *        index 0 should be `nullptr` since it cannot be used.
     * @param numKeywords number of keywords, including the nullptr in the 0th
     *        position. If greater than 32, will be truncated to 32.
     */
    explicit KString(
      const char* s,
      const char* const* keywords,
      uint8_t numKeywords
    ):
      string_(s),
      keywords_(keywords),
      type_(KString::kTypeCstring),
      numKeywords_(numKeywords > 0x20 ? 0x20 : numKeywords)
    {}

    /** Constructor around an Arduino Flash string. */
    explicit KString(
      const __FlashStringHelper* fs,
      const char* const* keywords,
      uint8_t numKeywords
    ):
      string_(fs),
      keywords_(keywords),
      type_(kTypeFstring),
      numKeywords_(numKeywords > 0x20 ? 0x20 : numKeywords)
    {}

    /**
     * Compare this string against a c-string `s` and return `<0`, `0` or `>0`
     * if this string is `<`, `==`, or `>` compared to the target string `s`. A
     * nullptr is considered to be smaller than any non-null string, including
     * the empty string.
     */
    int compareTo(const char* s);

    /**
     * Compare this string against another KString s and return `<0`, `0` or
     * `>0` if this string is `<`, `==`, or `>` compared to the target string
     * `s`.
     */
    int compareTo(const KString& s);

    /** Expand and print the current string to the given printer. */
    void printTo(Print& printer);

  private:
    static const uint8_t kTypeCstring = 0;
    static const uint8_t kTypeFstring = 1;

    // The order of the following fields is deliberate to reduce the memory
    // size of this class on 32-bit processors.
    const void* const string_;
    const char* const* const keywords_;
    uint8_t const type_;
    uint8_t const numKeywords_;
};

/**
 * An interator that points to a character inside a KString. This is essentially
 * a stack of 2 pointers. The first pointer points to the current string (either
 * the original string, or inside a fragment string. The second pointer is the
 * previous pointer, usually pointing to the original string. Each component
 * pointer can be either a c-string or an f-string, so there is a type
 * descriminator for each of the component pointer.
 */
class KStringIterator {
  public:
    /** Constructor. */
    KStringIterator(const KString& ks) :
        ks_(ks),
        firstType_(ks.type_),
        secondType_(KString::kTypeCstring),
        firstPtr_((const char*) ks.string_),
        secondPtr_(nullptr)
    {}

    /**
     * Return the current character referenced by the iterator. If the iterator
     * points to a compression token (i.e. c < 0x20), then the iterator moves
     * into the fragment string, and continues to return each character of the
     * fragment. When iterator hits the end of the fragment string (hits the NUL
     * character), the iterator returns to the original string, and continues
     * just after the compression token. At the end of the entire string, this
     * method returns a NUL to indicate the end of string.
     */
    char get() {
      // We don't support recursive compression fragments (i.e. compress tokens
      // within fragments) so this does NOT need to be a loop.
      char c = getInternal(firstType_, firstPtr_);
      if (c == '\0') {
        if (secondPtr_ != nullptr) {
          // pop the stack
          firstType_ = secondType_;
          firstPtr_ = secondPtr_;
          secondPtr_ = nullptr;

          // advance one character
          firstPtr_++;
          c = getInternal(firstType_, firstPtr_);
        }
      }

      if (c != '\0' && c < 0x20) { // fragment keyword string
        // push the stack
        secondType_ = firstType_;
        secondPtr_ = firstPtr_;
        firstType_ = KString::kTypeCstring;
        firstPtr_ = ks_.keywords_[(uint8_t) c];
        c = getInternal(firstType_, firstPtr_);
      }
      return c;
    }

    /** Advance the iterator one character, */
    void next() { firstPtr_++; }

  private:
    static char getInternal(uint8_t type, const char* p) {
      return (type == KString::kTypeCstring) ? *p : pgm_read_byte(p);
    }

  private:
    // ordering of fields optimized to reduce gaps on 32-bit processors
    const KString& ks_;
    uint8_t firstType_;
    uint8_t secondType_;
    const char* firstPtr_;
    const char* secondPtr_;
};

} // ace_common

#endif
