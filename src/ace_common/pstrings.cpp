#include <stdint.h>
#include <Arduino.h>
#include "pstrings.h"

namespace ace_common {

int strcmp_PP(const char* a, const char* b) {
  if (a == b) { return 0; }
  if (a == nullptr) { return -1; }
  if (b == nullptr) { return 1; }

  while (true) {
    uint8_t ca = pgm_read_byte(a);
    uint8_t cb = pgm_read_byte(b);
    if (ca != cb) return (int) ca - (int) cb;
    if (ca == '\0') return 0;
    a++;
    b++;
  }
}

}
