#include <Arduino.h>
#include <string.h>
#include "KString.h"

namespace ace_common {

int KString::compareTo(const char* s) {
  if (string_ == s) { return 0; }
  if (string_ == nullptr) { return -1; }
  if (s == nullptr) { return 1; }

  const char* a = (const char*) string_;
  const char* b = s;
  while (true) {
    uint8_t cb = *b;

    Restart:
    // Extract the current character depending on the 'stringType_'. Normally
    // doing a conditional check inside an inner loop is not good for
    // performance. However, when I pulled this code out into a template
    // function (using a thin-wrapper CString and FString), the
    // AceTime/AutoBenchmark program showed that it made no perceptible
    // difference in performance. I think this is because there is enough
    // overhead in the rest of function to make this conditional code
    // unimportant.
    uint8_t ca = (stringType_ == kTypeCstring) ? *a : pgm_read_byte(a);

    if (ca != cb) {
      // If ca is a keyword reference, then compare against the keyword.
      // Recursive keyword substitution not allowed, because I don't want to
      // make this function recursive.
      if (0 < ca && ca < numKeywords_) {
        KStringKeywords keywords(keywordType_, keywords_);
        const char* k = keywords.get(ca);
        while (true) {
          ca = (keywordType_ == kTypeCstring) ? *k : pgm_read_byte(k);
          cb = *b;
          if (ca == '\0') {
            a++;
            // Terrible goto, but it's the easiest way to bail out of the inner
            // loop and continue the outer loop at the correct character.
            goto Restart;
          }
          if (ca != cb) {
            return (int) ca - (int) cb;
          }
          k++;
          b++;
        }
      } else {
        return (int) ca - (int) cb;
      }
    }
    if (ca == '\0') return 0;
    a++;
    b++;
  }
}

int KString::compareTo(const KString& s) {
  if (this == &s) { return 0; }
  if (string_ == s.string_) { return 0; }
  if (string_ == nullptr) { return -1; }
  if (s.string_ == nullptr) { return 1; }

  KStringIterator aiter(*this);
  KStringIterator biter(s);
  while (true) {
    char ca = aiter.get();
    char cb = biter.get();

    if (ca == cb) {
      if (ca == '\0') {
        return 0;
      }
    } else {
      return ca - cb;
    }
    aiter.next();
    biter.next();
  }
}

void KString::printTo(Print& printer) {
  const char* s = (const char*) string_;
  if (s == nullptr) return;

  while (true) {
    // Same comment as above, doing a conditional check inside an inner loop is
    // usually not good for performance. But the templatized version of
    // compareTo() made no difference, and this function which outputs to a
    // Printer is not expected to be in a performance critical section.
    char c = (stringType_ == kTypeCstring) ? *s : pgm_read_byte(s);

    s++;
    if (c == 0) break;
    if (c < numKeywords_) {
      if (keywordType_ == kTypeCstring) {
        printer.print((const char*) keywords_[(uint8_t) c]);
      } else {
        printer.print((const __FlashStringHelper*)
            pgm_read_ptr(keywords_ + (uint8_t) c));
      }
    } else {
      printer.write(c);
    }
  }
}

char KStringIterator::get() {
  // We don't support recursive compression fragments (i.e. compress tokens
  // within fragments) so this does NOT need to be a loop.
  char c = getInternal(firstType_, firstPtr_);
  if (c == '\0') {
    if (secondPtr_ != nullptr) {
      // pop the stack
      firstPtr_ = secondPtr_;
      firstType_ = secondType_;
      secondPtr_ = nullptr;
      secondType_ = KString::kTypeCstring; // not used, can be anything

      // advance one character
      firstPtr_++;
      c = getInternal(firstType_, firstPtr_);
    }
  }

  if (c != '\0' && c < 0x20) { // fragment keyword string
    // push the stack
    secondPtr_ = firstPtr_;
    secondType_ = firstType_;

    KStringKeywords keywords(ks_.keywordType_, ks_.keywords_);
    firstPtr_ = keywords.get((uint8_t) c);
    firstType_ = ks_.keywordType_;
    c = getInternal(firstType_, firstPtr_);
  }

  return c;
}

} // ace_common
