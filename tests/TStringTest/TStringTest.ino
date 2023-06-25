#line 2 "TStringTest.ino"

#include <AceCommon.h>
#include <AUnitVerbose.h>

using namespace aunit;
using namespace ace_common;

test(strcat) {
  char st[10] = "01";
  const char s[] = "abc";
  strcat_T(st, s);
  assertEqual("01abc", st);

  char ft[10] = "12";
  const __FlashStringHelper* const f = F("abc");
  strcat_T(ft, f);
  assertEqual("12abc", ft);
}

test(strchr) {
  const char s[] = "abc";
  assertEqual((void*) (s + 1), strchr_T(s, 'b'));

  const __FlashStringHelper* const f = F("abc");
  assertEqual((void*) ((const char*)f + 1), strchr_T(f, 'b'));
}

test(strcmp) {
  const char s[] = "abc";
  assertMore(strcmp_T("abcd", s), 0);

  const __FlashStringHelper* const f = F("abc");
  assertMore(strcmp_T("abcd", f), 0);
}

test(strcpy) {
  char st[10];
  const char s[] = "abc";
  strcpy_T(st, s);
  assertEqual(st, "abc");

  char ft[10];
  const __FlashStringHelper* const f = F("abc");
  strcpy_T(ft, f);
  assertEqual("abc", ft);
}

test(strlen) {
  const char s[] = "abc";
  assertEqual((size_t)3, strlen_T(s));

  const __FlashStringHelper* const f = F("abc");
  assertEqual((size_t)3, strlen_T(f));
}

test(strncat) {
  char st[10] = "01";
  const char s[] = "abc";
  strncat_T(st, s, 2);
  assertEqual("01ab", st);

  char ft[10] = "12";
  const __FlashStringHelper* const f = F("abc");
  strncat_T(ft, f, 2);
  assertEqual("12ab", ft);
}

test(strncmp) {
  const char s[] = "abc";
  assertEqual(strncmp_T("abcd", s, 3), 0);

  const __FlashStringHelper* const f = F("abc");
  assertEqual(strncmp_T("abcd", f, 3), 0);
}

test(strncpy) {
  char st[10];
  const char s[] = "abc";
  strncpy_T(st, s, 2);
  st[2] = '\0';
  assertEqual(st, "ab");

  char ft[10];
  const __FlashStringHelper* const f = F("abc");
  strncpy_T(ft, f, 2);
  ft[2] = '\0';
  assertEqual("ab", ft);
}

test(strrchr) {
  const char s[] = "abcd";
  assertEqual((void*) (s + 2), strrchr_T(s, 'c'));

  const __FlashStringHelper* const f = F("abcd");
  assertEqual((void*) ((const char*)f + 2), strrchr_T(f, 'c'));
}

// ---------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // some boards reboot twice
#endif

  Serial.begin(115200);
  while (!Serial); // Leonardo/Micro

#if defined(EPOXY_DUINO)
  Serial.setLineModeUnix();
#endif
}

void loop() {
  TestRunner::run();
}
