#ifndef ACE_COMMON_PSTRINGS_H
#define ACE_COMMON_PSTRINGS_H

namespace ace_common {

/**
 * Compare 2 strings which are both stored in PROGMEM. This is similar to the
 * `strcmp_P(a, p)` except that both strings are in PROGMEM instead of just the
 * second string. Either `a` or `b` can be a `nullptr`. Two `nullptr` are
 * compared to be equal to each other. Any string is assumed to be "less than"
 * a `nullptr`.
 */
int strcmp_PP(const char* a, const char* b);

// These PROGMEM string functions exist in AVR but not in ESP8266 or ESP32.
#if defined(ESP8266) || defined(ESP32)

  /**
   * Return a pointer to the first occurence of `c` in the string `s` stored in
   * PROGMEM. Returns nullptr if not found.
   */
  const char* strchr_P(const char* s, int c);

  /**
   * Return a pointer to the last occurence of `c` in the string `s` stored in
   * PROGMEM. Returns nullptr if not found.
   */
  const char* strrchr_P(const char* s, int c);

#endif

}

#endif
