#line 2 "KStringTest.ino"

#include <AceCommon.h>
#include <AUnitVerbose.h>

using namespace aunit;
using ace_common::KString;
using ace_common::KStringIterator;
using ace_common::PrintStr;

const int NUM_KEYWORDS = 5;
const char* const KEYWORDS[NUM_KEYWORDS] = {
  nullptr,
  "Africa/", // \x01
  "America/", // \x02
  "Europe/", // \x03
  "Indiana/", // \x04
};

// ---------------------------------------------------------------------------
// compareTo(const char*)
// ---------------------------------------------------------------------------

test(KStringTest, compareTo_nullptr) {
  KString k1("America/", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02", KEYWORDS, NUM_KEYWORDS);
  KString k3("", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/"), KEYWORDS, NUM_KEYWORDS);
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

  assertEqual(k.compareTo("Africa/\x1f"), 0);
  assertEqual(kf.compareTo("Africa/\x1f"), 0);
}

test(KStringTest, compareTo_isolated) {
  KString k1("America/", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02"), KEYWORDS, NUM_KEYWORDS);

  assertLess(k1.compareTo("AmericaA"), 0);
  assertLess(k2.compareTo("AmericaA"), 0);
  assertEqual(k1.compareTo("America/"), 0);
  assertEqual(k2.compareTo("America/"), 0);
  assertMore(k1.compareTo("Americ"), 0);
  assertMore(k2.compareTo("Americ"), 0);

  assertLess(k1f.compareTo("AmericaA"), 0);
  assertLess(k2f.compareTo("AmericaA"), 0);
  assertEqual(k1f.compareTo("America/"), 0);
  assertEqual(k2f.compareTo("America/"), 0);
  assertMore(k1f.compareTo("Americ"), 0);
  assertMore(k2f.compareTo("Americ"), 0);
}

test(KStringTest, compareTo_prefix) {
  KString k1("America/Los_Angeles", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02Los_Angeles", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/Los_Angeles"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02Los_Angeles"), KEYWORDS, NUM_KEYWORDS);

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
  KString k1("America/Africa/", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02\x01", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/Africa/"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02\x01"), KEYWORDS, NUM_KEYWORDS);

  assertLess(k1.compareTo("America~"), 0);
  assertLess(k2.compareTo("America~"), 0);
  assertEqual(k1.compareTo("America/Africa/"), 0);
  assertEqual(k2.compareTo("America/Africa/"), 0);
  assertMore(k1.compareTo("America"), 0);
  assertMore(k2.compareTo("America"), 0);

  assertLess(k1f.compareTo("America~"), 0);
  assertLess(k2f.compareTo("America~"), 0);
  assertEqual(k1f.compareTo("America/Africa/"), 0);
  assertEqual(k2f.compareTo("America/Africa/"), 0);
  assertMore(k1f.compareTo("America"), 0);
  assertMore(k2f.compareTo("America"), 0);
}

// ---------------------------------------------------------------------------
// compareTo(const KString&)
// ---------------------------------------------------------------------------

test(KStringTest, compareTo_KString_nullptr) {
  KString k1("America/", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02", KEYWORDS, NUM_KEYWORDS);
  KString k3("", KEYWORDS, NUM_KEYWORDS);

  assertMore(k1.compareTo((const char*) nullptr), 0);
  assertMore(k2.compareTo((const char*) nullptr), 0);
  assertMore(k3.compareTo((const char*) nullptr), 0);

  KString k1f(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02"), KEYWORDS, NUM_KEYWORDS);
  KString k3f(F(""), KEYWORDS, NUM_KEYWORDS);

  assertMore(k1f.compareTo((const char*) nullptr), 0);
  assertMore(k2f.compareTo((const char*) nullptr), 0);
  assertMore(k3f.compareTo((const char*) nullptr), 0);
}

test(KStringTest, compareTo_KString_nullKString) {
  KString knull((const char*) nullptr, KEYWORDS, NUM_KEYWORDS);

  KString k1("America/", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02", KEYWORDS, NUM_KEYWORDS);
  KString k3("", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02"), KEYWORDS, NUM_KEYWORDS);
  KString k3f(F(""), KEYWORDS, NUM_KEYWORDS);

  assertMore(k1.compareTo(knull), 0);
  assertMore(k2.compareTo(knull), 0);
  assertMore(k3.compareTo(knull), 0);

  assertMore(k1f.compareTo(knull), 0);
  assertMore(k2f.compareTo(knull), 0);
  assertMore(k3f.compareTo(knull), 0);
}

test(KStringTest, compareTo_KString_KString) {
  KString k1("America/", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02", KEYWORDS, NUM_KEYWORDS);

  KString k1f(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString k2f(F("\x02"), KEYWORDS, NUM_KEYWORDS);

  assertEqual(k1.compareTo(k1), 0);
  assertEqual(k1.compareTo(k2), 0);
  assertEqual(k1.compareTo(k1f), 0);
  assertEqual(k1.compareTo(k2f), 0);

  assertEqual(k2.compareTo(k1), 0);
  assertEqual(k2.compareTo(k2), 0);
  assertEqual(k2.compareTo(k1f), 0);
  assertEqual(k2.compareTo(k2f), 0);

  assertEqual(k1f.compareTo(k1), 0);
  assertEqual(k1f.compareTo(k2), 0);
  assertEqual(k1f.compareTo(k1f), 0);
  assertEqual(k1f.compareTo(k2f), 0);

  assertEqual(k2f.compareTo(k1), 0);
  assertEqual(k2f.compareTo(k2), 0);
  assertEqual(k2f.compareTo(k1f), 0);
  assertEqual(k2f.compareTo(k2f), 0);
}

test(KStringTest, compareTo_KString_KString_two_consecutive_fragments) {
  KString k1("America/Indiana/Knox", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02\x04Knox", KEYWORDS, NUM_KEYWORDS);
  assertEqual(k1.compareTo(k2), 0);

  KString k3("America/Bahia_Banderas", KEYWORDS, NUM_KEYWORDS);
  assertMore(k1.compareTo(k3), 0);
  assertMore(k2.compareTo(k3), 0);

  KString k4("\x02" "Bahia_Banderas", KEYWORDS, NUM_KEYWORDS);
  assertMore(k1.compareTo(k4), 0);
  assertMore(k2.compareTo(k4), 0);
}

// ---------------------------------------------------------------------------
// printTo()
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

  KString k1("America/Africa/", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x02\x01", KEYWORDS, NUM_KEYWORDS);

  printStr.flush();
  k1.printTo(printStr);
  assertEqual(printStr.cstr(), "America/Africa/");

  printStr.flush();
  k2.printTo(printStr);
  assertEqual(printStr.cstr(), "America/Africa/");
}

// ---------------------------------------------------------------------------
// KStringIterator
// ---------------------------------------------------------------------------

test(KStringIteratorTest, oneLevelCString) {
  KString k("Africa", KEYWORDS, NUM_KEYWORDS);
  KStringIterator iter(k);

  assertEqual(iter.get(), 'A');
  iter.next();
  assertEqual(iter.get(), 'f');
  iter.next();
  assertEqual(iter.get(), 'r');
  iter.next();
  assertEqual(iter.get(), 'i');
}

test(KStringIteratorTest, oneLevelFString) {
  KString k(F("Africa"), KEYWORDS, NUM_KEYWORDS);
  KStringIterator iter(k);

  assertEqual(iter.get(), 'A');
  iter.next();
  assertEqual(iter.get(), 'f');
  iter.next();
  assertEqual(iter.get(), 'r');
  iter.next();
  assertEqual(iter.get(), 'i');
}

test(KStringIteratorTest, twoLevelCString) {
  // Africa/Lagos
  KString k("\x01Lagos", KEYWORDS, NUM_KEYWORDS);
  KStringIterator iter(k);

  assertEqual(iter.get(), 'A');
  iter.next();
  assertEqual(iter.get(), 'f');
  iter.next();
  assertEqual(iter.get(), 'r');
  iter.next();
  assertEqual(iter.get(), 'i');
  iter.next();
  assertEqual(iter.get(), 'c');
  iter.next();
  assertEqual(iter.get(), 'a');
  iter.next();
  assertEqual(iter.get(), '/');
  iter.next();
  assertEqual(iter.get(), 'L');
  iter.next();
  assertEqual(iter.get(), 'a');
  iter.next();
  assertEqual(iter.get(), 'g');
  iter.next();
  assertEqual(iter.get(), 'o');
  iter.next();
  assertEqual(iter.get(), 's');
  iter.next();
  assertEqual(iter.get(), '\0');
}

test(KStringIteratorTest, twoLevelFString) {
  // Africa/Lagos
  KString kf(F("\x01Lagos"), KEYWORDS, NUM_KEYWORDS);
  KStringIterator iter(kf);

  assertEqual(iter.get(), 'A');
  iter.next();
  assertEqual(iter.get(), 'f');
  iter.next();
  assertEqual(iter.get(), 'r');
  iter.next();
  assertEqual(iter.get(), 'i');
  iter.next();
  assertEqual(iter.get(), 'c');
  iter.next();
  assertEqual(iter.get(), 'a');
  iter.next();
  assertEqual(iter.get(), '/');
  iter.next();
  assertEqual(iter.get(), 'L');
  iter.next();
  assertEqual(iter.get(), 'a');
  iter.next();
  assertEqual(iter.get(), 'g');
  iter.next();
  assertEqual(iter.get(), 'o');
  iter.next();
  assertEqual(iter.get(), 's');
  iter.next();
  assertEqual(iter.get(), '\0');
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
