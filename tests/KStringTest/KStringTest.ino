#line 2 "KStringTest.ino"

#include <AceCommon.h>
#include <AUnitVerbose.h>

using namespace aunit;
using ace_common::KString;
using ace_common::PrintStr;

const int NUM_KEYWORDS = 4;
const char* const KEYWORDS[NUM_KEYWORDS] = {
  nullptr,
  "Africa",
  "America",
  "Europe",
};

// ---------------------------------------------------------------------------

test(KStringTest, compareTo_nullptr) {
  KString k1("America", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02", KEYWORDS, NUM_KEYWORDS);
  KString k3("", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02"), KEYWORDS, NUM_KEYWORDS);
  KString k3f(F(""), KEYWORDS, NUM_KEYWORDS);

  assertMore(k1.compareTo((const char*) nullptr), 0);
  assertMore(k2.compareTo((const char*) nullptr), 0);
  assertMore(k3.compareTo((const char*) nullptr), 0);

  assertMore(k1f.compareTo((const char*) nullptr), 0);
  assertMore(k2f.compareTo((const char*) nullptr), 0);
  assertMore(k3f.compareTo((const char*) nullptr), 0);
}

test(KStringTest, nullptr_compareTo) {
  KString k((const char*) nullptr, KEYWORDS, NUM_KEYWORDS);
  KString kf((const __FlashStringHelper*) nullptr, KEYWORDS, NUM_KEYWORDS);

  assertLess(k.compareTo(""), 0);
  assertLess(k.compareTo("America"), 0);
  assertEqual(k.compareTo((const char*) nullptr), 0);

  assertLess(kf.compareTo(""), 0);
  assertLess(kf.compareTo("America"), 0);
  assertEqual(kf.compareTo((const char*) nullptr), 0);
}

test(KStringTest, compareTo_emptyString) {
  KString k("", KEYWORDS, NUM_KEYWORDS);
  KString kf(F(""), KEYWORDS, NUM_KEYWORDS);

  assertMore(k.compareTo((const char*) nullptr), 0);
  assertEqual(k.compareTo(""), 0);
  assertLess(k.compareTo("America"), 0);

  assertMore(kf.compareTo((const char*) nullptr), 0);
  assertEqual(kf.compareTo(""), 0);
  assertLess(kf.compareTo("America"), 0);
}

test(KStringTest, compareTo_outOfBoundsReference) {
  // There are only 4 keywords in the dictionary, but the reference is to 31.
  // The compareTo() will treat the \x1f reference as a normal character.
  KString k("\x01\x1f", KEYWORDS, NUM_KEYWORDS);
  KString kf(F("\x01\x1f"), KEYWORDS, NUM_KEYWORDS);

  assertEqual(k.compareTo("Africa\x1f"), 0);
  assertEqual(kf.compareTo("Africa\x1f"), 0);
}

test(KStringTest, compareTo_isolated) {
  KString k1("America", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02"), KEYWORDS, NUM_KEYWORDS);

  assertLess(k1.compareTo("AmericaA"), 0);
  assertLess(k2.compareTo("AmericaA"), 0);
  assertEqual(k1.compareTo("America"), 0);
  assertEqual(k2.compareTo("America"), 0);
  assertMore(k1.compareTo("Americ"), 0);
  assertMore(k2.compareTo("Americ"), 0);

  assertLess(k1f.compareTo("AmericaA"), 0);
  assertLess(k2f.compareTo("AmericaA"), 0);
  assertEqual(k1f.compareTo("America"), 0);
  assertEqual(k2f.compareTo("America"), 0);
  assertMore(k1f.compareTo("Americ"), 0);
  assertMore(k2f.compareTo("Americ"), 0);
}

test(KStringTest, compareTo_prefix) {
  KString k1("America/Los_Angeles", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02/Los_Angeles", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/Los_Angeles"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02/Los_Angeles"), KEYWORDS, NUM_KEYWORDS);

  assertLess(k1.compareTo("America~"), 0);
  assertLess(k2.compareTo("America~"), 0);
  assertEqual(k1.compareTo("America/Los_Angeles"), 0);
  assertEqual(k2.compareTo("America/Los_Angeles"), 0);
  assertMore(k1.compareTo("America"), 0);
  assertMore(k2.compareTo("America"), 0);

  assertLess(k1f.compareTo("America~"), 0);
  assertLess(k2f.compareTo("America~"), 0);
  assertEqual(k1f.compareTo("America/Los_Angeles"), 0);
  assertEqual(k2f.compareTo("America/Los_Angeles"), 0);
  assertMore(k1f.compareTo("America"), 0);
  assertMore(k2f.compareTo("America"), 0);
}

test(KStringTest, compareTo_prefix_and_suffix) {
  KString k1("America/Africa", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02/\x01", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/Africa"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02/\x01"), KEYWORDS, NUM_KEYWORDS);

  assertLess(k1.compareTo("America~"), 0);
  assertLess(k2.compareTo("America~"), 0);
  assertEqual(k1.compareTo("America/Africa"), 0);
  assertEqual(k2.compareTo("America/Africa"), 0);
  assertMore(k1.compareTo("America"), 0);
  assertMore(k2.compareTo("America"), 0);

  assertLess(k1f.compareTo("America~"), 0);
  assertLess(k2f.compareTo("America~"), 0);
  assertEqual(k1f.compareTo("America/Africa"), 0);
  assertEqual(k2f.compareTo("America/Africa"), 0);
  assertMore(k1f.compareTo("America"), 0);
  assertMore(k2f.compareTo("America"), 0);
}

// ---------------------------------------------------------------------------

test(KStringTest, printTo_nullptr_empty) {
  PrintStr<20> printStr;

  KString nullKstr((const char*) nullptr, KEYWORDS, NUM_KEYWORDS);
  KString emptyKstr("", KEYWORDS, NUM_KEYWORDS);

  nullKstr.printTo(printStr);
  assertEqual(printStr.length(), (size_t) 0);

  emptyKstr.printTo(printStr);
  assertEqual(printStr.length(), (size_t) 0);
}

test(KStringTest, printTo_prefix_suffix) {
  PrintStr<20> printStr;

  KString k1("America/Africa", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02/\x01", KEYWORDS, NUM_KEYWORDS);

  printStr.flush();
  k1.printTo(printStr);
  assertEqual(printStr.cstr(), "America/Africa");

  printStr.flush();
  k2.printTo(printStr);
  assertEqual(printStr.cstr(), "America/Africa");
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
