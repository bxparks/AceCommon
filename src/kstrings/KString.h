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
  friend class KStringKeywords;

  public:
    /**
     * Constructor around a simple c-string, and an array of keyword strings in
     * normal memory.
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
      keywords_((const void* const*) keywords),
      stringType_(kTypeCstring),
      keywordType_(kTypeCstring),
      numKeywords_(numKeywords > 0x20 ? 0x20 : numKeywords)
    {}

    /**
     * Constructor around an f-string, and an array of keyword strings in normal
     * memory.
     *
     * @param s NUL terminated string or nullptr in flash memory
     * @param keywords an array of keywords in normal memory, up to 32 slots.
     *        The first slot at index 0 should be `nullptr` since it cannot be
     *        used.
     * @param numKeywords number of keywords, including the nullptr in the 0th
     *        position. If greater than 32, will be truncated to 32.
     */
    explicit KString(
      const __FlashStringHelper* s,
      const char* const* keywords,
      uint8_t numKeywords
    ):
      string_(s),
      keywords_((const void* const*) keywords),
      stringType_(kTypeFstring),
      keywordType_(kTypeCstring),
      numKeywords_(numKeywords > 0x20 ? 0x20 : numKeywords)
    {}

    /**
     * Constructor around a c-string, and an array of keyword strings in flash
     * memory.
     *
     * The Arduino type system does not have a way to properly represent
     * `keywords`. This parameter points to a location in flash memory, which
     * contains an array of pointers to strings also in flash memory. With the
     * current type system, `keywords` looks like a pointer to normal memory,
     * which contains an array of flash memory strings.
     *
     * @param s NUL terminated string or nullptr in normal memory
     * @param keywords an array of keywords in flash memory, up to 32 slots. The
     *        first slot at index 0 should be `nullptr` since it cannot be used.
     * @param numKeywords number of keywords, including the nullptr in the 0th
     *        position. If greater than 32, will be truncated to 32.
     */
    explicit KString(
      const char* s,
      const __FlashStringHelper* const* keywords,
      uint8_t numKeywords
    ):
      string_(s),
      keywords_((const void* const*) keywords),
      stringType_(kTypeCstring),
      keywordType_(kTypeFstring),
      numKeywords_(numKeywords > 0x20 ? 0x20 : numKeywords)
    {}

    /**
     * Constructor around an f-string, and an array of keyword strings in flash
     * memory.
     *
     * The Arduino type system does not have a way to properly represent
     * `keywords`. This parameter points to a location in flash memory, which
     * contains an array of pointers to strings also in flash memory. With the
     * current type system, `keywords` looks like a pointer to normal memory,
     * which contains an array of flash memory strings.
     *
     * @param s NUL terminated string or nullptr in flash memory
     * @param keywords an array of keywords, up to 32 slots. The first slot at
     *        index 0 should be `nullptr` since it cannot be used.
     * @param numKeywords number of keywords, including the nullptr in the 0th
     *        position. If greater than 32, will be truncated to 32.
     */
    explicit KString(
      const __FlashStringHelper* s,
      const __FlashStringHelper* const* keywords,
      uint8_t numKeywords
    ):
      string_(s),
      keywords_((const void* const*) keywords),
      stringType_(kTypeFstring),
      keywordType_(kTypeFstring),
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
    const void* const* const keywords_;
    uint8_t const stringType_;
    uint8_t const keywordType_;
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
        firstPtr_((const char*) ks.string_),
        secondPtr_(nullptr),
        firstType_(ks.stringType_),
        secondType_(KString::kTypeCstring) // initial value can be anything
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
    char get();

    /** Advance the iterator one character, */
    void next() { firstPtr_++; }

  private:
    static char getInternal(uint8_t type, const char* p) {
      return (type == KString::kTypeCstring) ? *p : pgm_read_byte(p);
    }

  private:
    // ordering of fields optimized to reduce gaps on 32-bit processors
    const KString& ks_;
    const char* firstPtr_;
    const char* secondPtr_;
    uint8_t firstType_;
    uint8_t secondType_;
};

/**
 * A thin helper object around an array of `const char*` in regular memory, or
 * an array of `const __FlashStringHelper*` in flash memory. Simplifies some
 * code in `KString.cpp`.
 */
class KStringKeywords {
  public:
    /**
     * @param type storage type of `keywords`, either KString::kTypeCstring
     *        or KString::kTypeFstring
     * @param keywords an array of pointers to strings, either in normal memory
     *        or flash memory
     */
    KStringKeywords(uint8_t type, const void* const* keywords) :
      type_(type),
      keywords_(keywords)
    {}

    /**
     * Return the string pointer of index i. The actual pointer type is either
     * cstring or an fstring, depending on the `type` passed into the
     * constructor.
     */
    const char* get(uint8_t i) const {
      if (type_ == KString::kTypeCstring) {
        auto words = (const char* const*) keywords_;
        return words[i];
      } else {
        auto words = (const __FlashStringHelper* const*) keywords_;
        return (const char*) pgm_read_ptr(words + i);
      }
    }

  private:
    uint8_t type_;
    const void* const* keywords_;
};

} // ace_common

#endif
