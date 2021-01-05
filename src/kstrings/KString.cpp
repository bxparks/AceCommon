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
  if (cstring_ == s) { return 0; }
  if (cstring_ == nullptr) { return -1; }
  if (s == nullptr) { return 1; }

  const char* a = cstring_;
  const char* b = s;
  while (true) {
    uint8_t ca;
    uint8_t cb;
    cb = *b;
    Restart:
    ca = *a;
    #if ENABLE_SERIAL_DEBUG
      printfTo(Serial, "outer: a=%c b=%c", ca, cb);
      Serial.println();
    #endif
    if (ca != cb) {
      // If ca is a keyword reference, then compare against the keyword.
      // Recursive keyword substitution not allowed, because I don't want to
      // make this function recursive.
      if (0 < ca && ca < 0x20) {
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
  const char* s = cstring_;
  if (s == nullptr) return;
  while (true) {
    char c = *s++;
    if (c == 0) break;
    if (c < 0x20) {
      printer.print(keywords_[c]);
    } else {
      printer.write(c);
    }
  }
}

} // ace_common
