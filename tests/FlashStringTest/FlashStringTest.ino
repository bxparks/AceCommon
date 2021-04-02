#line 2 "FlashStringTest.ino"

#include <AceCommon.h>
#include <AUnitVerbose.h>

using aunit::TestRunner;
using ace_common::FlashString;

// ---------------------------------------------------------------------------

test(FlashString, assignmentAndCopyConstructor) {
  FlashString fs(F("the string"));
  FlashString fs2(F("the string 2"));

  // Copy constructor
  FlashString s(fs);
  assertEqual(s, fs);

  // Assignment operator.
  s = fs2;
  assertEqual(s, fs2);
}

test(FlashString, dereference) {
  FlashString fs = F("the string");
  assertEqual('t', *fs);
  assertEqual('t', fs[0]);
  assertEqual('h', fs[1]);
  assertEqual('e', fs[2]);
  assertEqual(' ', fs[3]);
}

test(FlashString, incrementAndDecrement) {
  const char STRING[] = "the string";
  FlashString fs = F("the string");
  const char* cs = STRING;

  for (size_t i = 0; i < sizeof(STRING); ++i) {
    assertEqual(*fs++, *cs++);
  }
  for (size_t i = 0; i < sizeof(STRING); ++i) {
    assertEqual(*--fs, *--cs);
  }
  for (size_t i = 0; i < sizeof(STRING) - 1; ++i) {
    assertEqual(*++fs, *++cs);
  }
  for (size_t i = 0; i < sizeof(STRING) - 1; ++i) {
    assertEqual(*fs--, *cs--);
  }
}

test(FlashString, comparisons_work_by_casting) {
  const __FlashStringHelper* FSTRING = F("the string");
  FlashString fs = FSTRING;

  // Test explicit casting to (const __FlashStringHelper*).
  const __FlashStringHelper* fstring = (const __FlashStringHelper*) fs;
  assertEqual(FSTRING, fstring);

  // Test comparision with nullptr.
  assertTrue(fs != nullptr);
  assertTrue(nullptr != fs);

  // Create a 2nd FlashString.
  const __FlashStringHelper* FSTRING2 = F("the string 2");
  const FlashString fs2 = FSTRING2;

  // Test == and != work.
  assertTrue(! (fs == fs2));
  assertTrue(fs != fs2);

  // Test implicit conversion to bool.
  if (fs) {
    pass();
  } else {
    failTestNow();
  }
  if (!fs) {
    failTestNow();
  } else {
    pass();
  }
}

// ---------------------------------------------------------------------------

void setup() {
#if defined(ARDUINO)
  delay(1000); // some boards reboot twice
#endif

  Serial.begin(115200);
  while (!Serial); // Leonardo/Micro
}

void loop() {
  TestRunner::run();
}
