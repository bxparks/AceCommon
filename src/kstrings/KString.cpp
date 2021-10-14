#include <Arduino.h>
#include <string.h>
#include "KString.h"

#if ENABLE_SERIAL_DEBUG
  #include "../print_utils/printfTo.h"
#endif

namespace ace_common {

int KString::compareTo(const char* s) {
  #if ENABLE_SERIAL_DEBUG
    Serial.println("compareTo(const char*)");
  #endif
  if (string_ == s) { return 0; }
  if (string_ == nullptr) { return -1; }
  if (s == nullptr) { return 1; }

  const char* a = (const char*) string_;
  const char* b = s;
  bool isCstring = (type_ == kTypeCstring);
  while (true) {
    uint8_t cb = *b;

    Restart:
    // Extract the current character depending on the 'type_'. Normally doing a
    // conditional check inside an inner loop is not good for performance.
    // However, when I pulled this code out into a template function (using a
    // thin-wrapper CString and FString), the AceTime/AutoBenchmark program
    // showed that it made no perceptible difference in performance. I think
    // this is because there is enough overhead in the rest of function to make
    // this conditional code unimportant.
    uint8_t ca = isCstring ? *a : pgm_read_byte(a);

    #if ENABLE_SERIAL_DEBUG
      printfTo(Serial, "outer: a=%c b=%c", ca, cb);
      Serial.println();
    #endif
    if (ca != cb) {
      // If ca is a keyword reference, then compare against the keyword.
      // Recursive keyword substitution not allowed, because I don't want to
      // make this function recursive.
      if (0 < ca && ca < numKeywords_) {
        const char* k = keywords_[ca];
        while (true) {
          ca = *k;
          cb = *b;
          #if ENABLE_SERIAL_DEBUG
            printfTo(Serial, "inner: a=%c b=%c", ca, cb);
            Serial.println();
          #endif
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
  #if ENABLE_SERIAL_DEBUG
    Serial.println("compareTo(const KString&)");
  #endif
  if (this == &s) { return 0; }
  if (string_ == s.string_) { return 0; }
  if (string_ == nullptr) { return -1; }
  if (s.string_ == nullptr) { return 1; }

  KStringIterator aiter(*this);
  KStringIterator biter(s);
  while (true) {
    #if ENABLE_SERIAL_DEBUG
      printfTo(Serial, "outer: a=%c b=%c", ca, cb);
      Serial.println();
    #endif

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

  bool isCstring = (type_ == kTypeCstring);
  while (true) {
    // Same comment as above, doing a conditional check inside an inner loop is
    // usually not good for performance. But the templatized version of
    // compareTo() made no difference, and this function which outputs to a
    // Printer is not expected to be in a performance critical section.
    char c = isCstring ? *s : pgm_read_byte(s);

    s++;
    if (c == 0) break;
    if (c < numKeywords_) {
      printer.print(keywords_[(uint8_t) c]);
    } else {
      printer.write(c);
    }
  }
}

} // ace_common
