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

}

#endif
