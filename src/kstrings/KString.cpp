#include <Arduino.h>
#include <string.h>
#include "KString.h"

#if ENABLE_SERIAL_DEBUG
  #include "../print_utils/printfTo.h"
#endif

namespace ace_common {

int KString::compareTo(const char* s) {
  #if ENABLE_SERIAL_DEBUG
    Serial.println("compareTo()");
  #endif
  if (string_ == s) { return 0; }
  if (string_ == nullptr) { return -1; }
  if (s == nullptr) { return 1; }

  const char* a = (const char*) string_;
  const char* b = s;
  bool isCstring = (type_ == kTypeCstring);
  while (true) {
    uint8_t cb = *b;

    // Extract the current character depending on the 'type_'. Note that this
    // is terrible for performance, since the if-statement is executed on every
    // iteration. However, the primary purpose of the KString class is to
    // reduce flash memory usage, so I don't want to create a second version of
    // this function that differs only in this statement. The assumption is
    // that objects of this class are accessed only rarely, preferrably outside
    // of critical loops.
    Restart:
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

void KString::printTo(Print& printer) {
  const char* s = (const char*) string_;
  if (s == nullptr) return;

  bool isCstring = (type_ == kTypeCstring);
  while (true) {
    // Same comment as above, this is terrible for performance, but keeps the
    // code size small.
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
