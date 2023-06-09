#line 2 "KStringTest.ino"

#include <AceCommon.h>
#include <AUnitVerbose.h>

using namespace aunit;
using ace_common::KString;
using ace_common::KStringIterator;
using ace_common::PrintStr;

const int NUM_KEYWORDS = 5;

// Keywords in regular memory.
const char* const KEYWORDS[NUM_KEYWORDS] = {
  nullptr,
  "Africa/", // \x01
  "America/", // \x02
  "Europe/", // \x03
  "Indiana/", // \x04
};

static const char kFragment1[] PROGMEM = "Africa/";
static const char kFragment2[] PROGMEM = "America/";
static const char kFragment3[] PROGMEM = "Europe/";
static const char kFragment4[] PROGMEM = "Indiana/";

// Keywords in flash memory.
const __FlashStringHelper* const FLASHWORDS[NUM_KEYWORDS] PROGMEM = {
  nullptr,
  (const __FlashStringHelper*) kFragment1,
  (const __FlashStringHelper*) kFragment2,
  (const __FlashStringHelper*) kFragment3,
  (const __FlashStringHelper*) kFragment4,
};

// ---------------------------------------------------------------------------
// compareTo(const char*)
// ---------------------------------------------------------------------------

test(KStringTest, compareTo_nullptr) {
  KString ss1("America/", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02", KEYWORDS, NUM_KEYWORDS);
  KString ss3("", KEYWORDS, NUM_KEYWORDS);

  KString fs1(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02"), KEYWORDS, NUM_KEYWORDS);
  KString fs3(F(""), KEYWORDS, NUM_KEYWORDS);

  KString sf1("America/", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02", FLASHWORDS, NUM_KEYWORDS);
  KString sf3("", FLASHWORDS, NUM_KEYWORDS);

  KString ff1(F("America/"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02"), FLASHWORDS, NUM_KEYWORDS);
  KString ff3(F(""), FLASHWORDS, NUM_KEYWORDS);

  assertMore(ss1.compareTo((const char*) nullptr), 0);
  assertMore(ss2.compareTo((const char*) nullptr), 0);
  assertMore(ss3.compareTo((const char*) nullptr), 0);

  assertMore(fs1.compareTo((const char*) nullptr), 0);
  assertMore(fs2.compareTo((const char*) nullptr), 0);
  assertMore(fs3.compareTo((const char*) nullptr), 0);

  assertMore(sf1.compareTo((const char*) nullptr), 0);
  assertMore(sf2.compareTo((const char*) nullptr), 0);
  assertMore(sf3.compareTo((const char*) nullptr), 0);

  assertMore(ff1.compareTo((const char*) nullptr), 0);
  assertMore(ff2.compareTo((const char*) nullptr), 0);
  assertMore(ff3.compareTo((const char*) nullptr), 0);
}

test(KStringTest, nullptr_compareTo) {
  KString ss((const char*) nullptr, KEYWORDS, NUM_KEYWORDS);
  KString fs((const __FlashStringHelper*) nullptr, KEYWORDS, NUM_KEYWORDS);
  KString sf((const char*) nullptr, FLASHWORDS, NUM_KEYWORDS);
  KString ff((const __FlashStringHelper*) nullptr, FLASHWORDS, NUM_KEYWORDS);

  assertLess(ss.compareTo(""), 0);
  assertLess(ss.compareTo("America"), 0);
  assertEqual(ss.compareTo((const char*) nullptr), 0);

  assertLess(fs.compareTo(""), 0);
  assertLess(fs.compareTo("America"), 0);
  assertEqual(fs.compareTo((const char*) nullptr), 0);

  assertLess(sf.compareTo(""), 0);
  assertLess(sf.compareTo("America"), 0);
  assertEqual(sf.compareTo((const char*) nullptr), 0);

  assertLess(ff.compareTo(""), 0);
  assertLess(ff.compareTo("America"), 0);
  assertEqual(ff.compareTo((const char*) nullptr), 0);
}

test(KStringTest, compareTo_emptyString) {
  KString ss("", KEYWORDS, NUM_KEYWORDS);
  KString fs(F(""), KEYWORDS, NUM_KEYWORDS);
  KString sf("", FLASHWORDS, NUM_KEYWORDS);
  KString ff(F(""), FLASHWORDS, NUM_KEYWORDS);

  assertMore(ss.compareTo((const char*) nullptr), 0);
  assertEqual(ss.compareTo(""), 0);
  assertLess(ss.compareTo("America"), 0);

  assertMore(fs.compareTo((const char*) nullptr), 0);
  assertEqual(fs.compareTo(""), 0);
  assertLess(fs.compareTo("America"), 0);

  assertMore(sf.compareTo((const char*) nullptr), 0);
  assertEqual(sf.compareTo(""), 0);
  assertLess(sf.compareTo("America"), 0);

  assertMore(ff.compareTo((const char*) nullptr), 0);
  assertEqual(ff.compareTo(""), 0);
  assertLess(ff.compareTo("America"), 0);
}

test(KStringTest, compareTo_outOfBoundsReference) {
  // There are only 4 keywords in the dictionary, but the reference is to 31.
  // The compareTo() will treat the \x1f reference as a normal character.
  KString ss("\x01\x1f", KEYWORDS, NUM_KEYWORDS);
  KString fs(F("\x01\x1f"), KEYWORDS, NUM_KEYWORDS);
  KString sf("\x01\x1f", FLASHWORDS, NUM_KEYWORDS);
  KString ff(F("\x01\x1f"), FLASHWORDS, NUM_KEYWORDS);

  assertEqual(ss.compareTo("Africa/\x1f"), 0);
  assertEqual(fs.compareTo("Africa/\x1f"), 0);
  assertEqual(sf.compareTo("Africa/\x1f"), 0);
  assertEqual(ff.compareTo("Africa/\x1f"), 0);
}

test(KStringTest, compareTo_isolated) {
  KString ss1("America/", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02", KEYWORDS, NUM_KEYWORDS);

  KString fs1(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02"), KEYWORDS, NUM_KEYWORDS);

  KString sf1("America/", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02", FLASHWORDS, NUM_KEYWORDS);

  KString ff1(F("America/"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02"), FLASHWORDS, NUM_KEYWORDS);

  assertLess(ss1.compareTo("AmericaA"), 0);
  assertLess(ss2.compareTo("AmericaA"), 0);
  assertEqual(ss1.compareTo("America/"), 0);
  assertEqual(ss2.compareTo("America/"), 0);
  assertMore(ss1.compareTo("Americ"), 0);
  assertMore(ss2.compareTo("Americ"), 0);

  assertLess(fs1.compareTo("AmericaA"), 0);
  assertLess(fs2.compareTo("AmericaA"), 0);
  assertEqual(fs1.compareTo("America/"), 0);
  assertEqual(fs2.compareTo("America/"), 0);
  assertMore(fs1.compareTo("Americ"), 0);
  assertMore(fs2.compareTo("Americ"), 0);

  assertLess(sf1.compareTo("AmericaA"), 0);
  assertLess(sf2.compareTo("AmericaA"), 0);
  assertEqual(sf1.compareTo("America/"), 0);
  assertEqual(sf2.compareTo("America/"), 0);
  assertMore(sf1.compareTo("Americ"), 0);
  assertMore(sf2.compareTo("Americ"), 0);

  assertLess(ff1.compareTo("AmericaA"), 0);
  assertLess(ff2.compareTo("AmericaA"), 0);
  assertEqual(ff1.compareTo("America/"), 0);
  assertEqual(ff2.compareTo("America/"), 0);
  assertMore(ff1.compareTo("Americ"), 0);
  assertMore(ff2.compareTo("Americ"), 0);
}

test(KStringTest, compareTo_prefix) {
  KString ss1("America/Los_Angeles", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02Los_Angeles", KEYWORDS, NUM_KEYWORDS);

  KString fs1(F("America/Los_Angeles"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02Los_Angeles"), KEYWORDS, NUM_KEYWORDS);

  KString sf1("America/Los_Angeles", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02Los_Angeles", FLASHWORDS, NUM_KEYWORDS);

  KString ff1(F("America/Los_Angeles"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02Los_Angeles"), FLASHWORDS, NUM_KEYWORDS);

  assertLess(ss1.compareTo("America~"), 0);
  assertLess(ss2.compareTo("America~"), 0);
  assertEqual(ss1.compareTo("America/Los_Angeles"), 0);
  assertEqual(ss2.compareTo("America/Los_Angeles"), 0);
  assertMore(ss1.compareTo("America"), 0);
  assertMore(ss2.compareTo("America"), 0);

  assertLess(fs1.compareTo("America~"), 0);
  assertLess(fs2.compareTo("America~"), 0);
  assertEqual(fs1.compareTo("America/Los_Angeles"), 0);
  assertEqual(fs2.compareTo("America/Los_Angeles"), 0);
  assertMore(fs1.compareTo("America"), 0);
  assertMore(fs2.compareTo("America"), 0);

  assertLess(sf1.compareTo("America~"), 0);
  assertLess(sf2.compareTo("America~"), 0);
  assertEqual(sf1.compareTo("America/Los_Angeles"), 0);
  assertEqual(sf2.compareTo("America/Los_Angeles"), 0);
  assertMore(sf1.compareTo("America"), 0);
  assertMore(sf2.compareTo("America"), 0);

  assertLess(ff1.compareTo("America~"), 0);
  assertLess(ff2.compareTo("America~"), 0);
  assertEqual(ff1.compareTo("America/Los_Angeles"), 0);
  assertEqual(ff2.compareTo("America/Los_Angeles"), 0);
  assertMore(ff1.compareTo("America"), 0);
  assertMore(ff2.compareTo("America"), 0);
}

test(KStringTest, compareTo_prefix_and_suffix) {
  KString ss1("America/Africa/", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02\x01", KEYWORDS, NUM_KEYWORDS);

  KString fs1(F("America/Africa/"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02\x01"), KEYWORDS, NUM_KEYWORDS);

  KString sf1("America/Africa/", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02\x01", FLASHWORDS, NUM_KEYWORDS);

  KString ff1(F("America/Africa/"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02\x01"), FLASHWORDS, NUM_KEYWORDS);

  assertLess(ss1.compareTo("America~"), 0);
  assertLess(ss2.compareTo("America~"), 0);
  assertEqual(ss1.compareTo("America/Africa/"), 0);
  assertEqual(ss2.compareTo("America/Africa/"), 0);
  assertMore(ss1.compareTo("America"), 0);
  assertMore(ss2.compareTo("America"), 0);

  assertLess(fs1.compareTo("America~"), 0);
  assertLess(fs2.compareTo("America~"), 0);
  assertEqual(fs1.compareTo("America/Africa/"), 0);
  assertEqual(fs2.compareTo("America/Africa/"), 0);
  assertMore(fs1.compareTo("America"), 0);
  assertMore(fs2.compareTo("America"), 0);

  assertLess(sf1.compareTo("America~"), 0);
  assertLess(sf2.compareTo("America~"), 0);
  assertEqual(sf1.compareTo("America/Africa/"), 0);
  assertEqual(sf2.compareTo("America/Africa/"), 0);
  assertMore(sf1.compareTo("America"), 0);
  assertMore(sf2.compareTo("America"), 0);

  assertLess(ff1.compareTo("America~"), 0);
  assertLess(ff2.compareTo("America~"), 0);
  assertEqual(ff1.compareTo("America/Africa/"), 0);
  assertEqual(ff2.compareTo("America/Africa/"), 0);
  assertMore(ff1.compareTo("America"), 0);
  assertMore(ff2.compareTo("America"), 0);
}

// ---------------------------------------------------------------------------
// compareTo(const KString&)
// ---------------------------------------------------------------------------

test(KStringTest, compareTo_KString_nullptr) {
  KString ss1("America/", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02", KEYWORDS, NUM_KEYWORDS);
  KString ss3("", KEYWORDS, NUM_KEYWORDS);

  KString fs1(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02"), KEYWORDS, NUM_KEYWORDS);
  KString fs3(F(""), KEYWORDS, NUM_KEYWORDS);

  KString sf1("America/", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02", FLASHWORDS, NUM_KEYWORDS);
  KString sf3("", FLASHWORDS, NUM_KEYWORDS);

  KString ff1(F("America/"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02"), FLASHWORDS, NUM_KEYWORDS);
  KString ff3(F(""), FLASHWORDS, NUM_KEYWORDS);

  assertMore(ss1.compareTo((const char*) nullptr), 0);
  assertMore(ss2.compareTo((const char*) nullptr), 0);
  assertMore(ss3.compareTo((const char*) nullptr), 0);

  assertMore(fs1.compareTo((const char*) nullptr), 0);
  assertMore(fs2.compareTo((const char*) nullptr), 0);
  assertMore(fs3.compareTo((const char*) nullptr), 0);

  assertMore(sf1.compareTo((const char*) nullptr), 0);
  assertMore(sf2.compareTo((const char*) nullptr), 0);
  assertMore(sf3.compareTo((const char*) nullptr), 0);

  assertMore(ff1.compareTo((const char*) nullptr), 0);
  assertMore(ff2.compareTo((const char*) nullptr), 0);
  assertMore(ff3.compareTo((const char*) nullptr), 0);
}

test(KStringTest, compareTo_KString_nullKString) {
  KString knull((const char*) nullptr, KEYWORDS, NUM_KEYWORDS);

  KString ss1("America/", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02", KEYWORDS, NUM_KEYWORDS);
  KString ss3("", KEYWORDS, NUM_KEYWORDS);

  KString fs1(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02"), KEYWORDS, NUM_KEYWORDS);
  KString fs3(F(""), KEYWORDS, NUM_KEYWORDS);

  KString sf1("America/", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02", FLASHWORDS, NUM_KEYWORDS);
  KString sf3("", FLASHWORDS, NUM_KEYWORDS);

  KString ff1(F("America/"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02"), FLASHWORDS, NUM_KEYWORDS);
  KString ff3(F(""), FLASHWORDS, NUM_KEYWORDS);

  assertMore(ss1.compareTo(knull), 0);
  assertMore(ss2.compareTo(knull), 0);
  assertMore(ss3.compareTo(knull), 0);

  assertMore(fs1.compareTo(knull), 0);
  assertMore(fs2.compareTo(knull), 0);
  assertMore(fs3.compareTo(knull), 0);

  assertMore(sf1.compareTo(knull), 0);
  assertMore(sf2.compareTo(knull), 0);
  assertMore(sf3.compareTo(knull), 0);

  assertMore(ff1.compareTo(knull), 0);
  assertMore(ff2.compareTo(knull), 0);
  assertMore(ff3.compareTo(knull), 0);
}

test(KStringTest, compareTo_KString_KString) {
  KString ss1("America/", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02", KEYWORDS, NUM_KEYWORDS);

  KString fs1(F("America/"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02"), KEYWORDS, NUM_KEYWORDS);

  KString sf1("America/", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02", FLASHWORDS, NUM_KEYWORDS);

  KString ff1(F("America/"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02"), FLASHWORDS, NUM_KEYWORDS);

  assertEqual(ss1.compareTo(ss1), 0);
  assertEqual(ss1.compareTo(ss2), 0);
  assertEqual(ss1.compareTo(fs1), 0);
  assertEqual(ss1.compareTo(fs2), 0);
  assertEqual(ss1.compareTo(sf1), 0);
  assertEqual(ss1.compareTo(sf2), 0);
  assertEqual(ss1.compareTo(ff1), 0);
  assertEqual(ss1.compareTo(ff2), 0);

  assertEqual(ss2.compareTo(ss1), 0);
  assertEqual(ss2.compareTo(ss2), 0);
  assertEqual(ss2.compareTo(fs1), 0);
  assertEqual(ss2.compareTo(fs2), 0);
  assertEqual(ss2.compareTo(sf1), 0);
  assertEqual(ss2.compareTo(sf2), 0);
  assertEqual(ss2.compareTo(ff1), 0);
  assertEqual(ss2.compareTo(ff2), 0);

  assertEqual(fs1.compareTo(ss1), 0);
  assertEqual(fs1.compareTo(ss2), 0);
  assertEqual(fs1.compareTo(fs1), 0);
  assertEqual(fs1.compareTo(fs2), 0);
  assertEqual(fs1.compareTo(sf1), 0);
  assertEqual(fs1.compareTo(sf2), 0);
  assertEqual(fs1.compareTo(ff1), 0);
  assertEqual(fs1.compareTo(ff2), 0);

  assertEqual(fs2.compareTo(ss1), 0);
  assertEqual(fs2.compareTo(ss2), 0);
  assertEqual(fs2.compareTo(fs1), 0);
  assertEqual(fs2.compareTo(fs2), 0);
  assertEqual(fs2.compareTo(sf1), 0);
  assertEqual(fs2.compareTo(sf2), 0);
  assertEqual(fs2.compareTo(ff1), 0);
  assertEqual(fs2.compareTo(ff2), 0);

  assertEqual(sf1.compareTo(ss1), 0);
  assertEqual(sf1.compareTo(ss2), 0);
  assertEqual(sf1.compareTo(fs1), 0);
  assertEqual(sf1.compareTo(fs2), 0);
  assertEqual(sf1.compareTo(sf1), 0);
  assertEqual(sf1.compareTo(sf2), 0);
  assertEqual(sf1.compareTo(ff1), 0);
  assertEqual(sf1.compareTo(ff2), 0);

  assertEqual(sf2.compareTo(ss1), 0);
  assertEqual(sf2.compareTo(ss2), 0);
  assertEqual(sf2.compareTo(fs1), 0);
  assertEqual(sf2.compareTo(fs2), 0);
  assertEqual(sf2.compareTo(sf1), 0);
  assertEqual(sf2.compareTo(sf2), 0);
  assertEqual(sf2.compareTo(ff1), 0);
  assertEqual(sf2.compareTo(ff2), 0);

  assertEqual(ff1.compareTo(ss1), 0);
  assertEqual(ff1.compareTo(ss2), 0);
  assertEqual(ff1.compareTo(fs1), 0);
  assertEqual(ff1.compareTo(fs2), 0);
  assertEqual(ff1.compareTo(sf1), 0);
  assertEqual(ff1.compareTo(sf2), 0);
  assertEqual(ff1.compareTo(ff1), 0);
  assertEqual(ff1.compareTo(ff2), 0);

  assertEqual(ff2.compareTo(ss1), 0);
  assertEqual(ff2.compareTo(ss2), 0);
  assertEqual(ff2.compareTo(fs1), 0);
  assertEqual(ff2.compareTo(fs2), 0);
  assertEqual(ff2.compareTo(sf1), 0);
  assertEqual(ff2.compareTo(sf2), 0);
  assertEqual(ff2.compareTo(ff1), 0);
  assertEqual(ff2.compareTo(ff2), 0);
}

test(KStringTest, compareTo_KString_KString_two_consecutive_fragments) {
  KString ss1("America/Indiana/Knox", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02\x04Knox", KEYWORDS, NUM_KEYWORDS);
  KString fs1(F("America/Indiana/Knox"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02\x04Knox"), KEYWORDS, NUM_KEYWORDS);
  KString sf1("America/Indiana/Knox", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02\x04Knox", FLASHWORDS, NUM_KEYWORDS);
  KString ff1(F("America/Indiana/Knox"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02\x04Knox"), FLASHWORDS, NUM_KEYWORDS);

  assertEqual(ss1.compareTo(ss1), 0);
  assertEqual(ss1.compareTo(ss2), 0);
  assertEqual(ss1.compareTo(fs1), 0);
  assertEqual(ss1.compareTo(fs2), 0);
  assertEqual(ss1.compareTo(sf1), 0);
  assertEqual(ss1.compareTo(sf2), 0);
  assertEqual(ss1.compareTo(ff1), 0);
  assertEqual(ss1.compareTo(ff2), 0);

  assertEqual(ss2.compareTo(ss1), 0);
  assertEqual(ss2.compareTo(ss2), 0);
  assertEqual(ss2.compareTo(fs1), 0);
  assertEqual(ss2.compareTo(fs2), 0);
  assertEqual(ss2.compareTo(sf1), 0);
  assertEqual(ss2.compareTo(sf2), 0);
  assertEqual(ss2.compareTo(ff1), 0);
  assertEqual(ss2.compareTo(ff2), 0);

  assertEqual(fs1.compareTo(ss1), 0);
  assertEqual(fs1.compareTo(ss2), 0);
  assertEqual(fs1.compareTo(fs1), 0);
  assertEqual(fs1.compareTo(fs2), 0);
  assertEqual(fs1.compareTo(sf1), 0);
  assertEqual(fs1.compareTo(sf2), 0);
  assertEqual(fs1.compareTo(ff1), 0);
  assertEqual(fs1.compareTo(ff2), 0);

  assertEqual(fs2.compareTo(ss1), 0);
  assertEqual(fs2.compareTo(ss2), 0);
  assertEqual(fs2.compareTo(fs1), 0);
  assertEqual(fs2.compareTo(fs2), 0);
  assertEqual(fs2.compareTo(sf1), 0);
  assertEqual(fs2.compareTo(sf2), 0);
  assertEqual(fs2.compareTo(ff1), 0);
  assertEqual(fs2.compareTo(ff2), 0);

  assertEqual(sf1.compareTo(ss1), 0);
  assertEqual(sf1.compareTo(ss2), 0);
  assertEqual(sf1.compareTo(fs1), 0);
  assertEqual(sf1.compareTo(fs2), 0);
  assertEqual(sf1.compareTo(sf1), 0);
  assertEqual(sf1.compareTo(sf2), 0);
  assertEqual(sf1.compareTo(ff1), 0);
  assertEqual(sf1.compareTo(ff2), 0);

  assertEqual(sf2.compareTo(ss1), 0);
  assertEqual(sf2.compareTo(ss2), 0);
  assertEqual(sf2.compareTo(fs1), 0);
  assertEqual(sf2.compareTo(fs2), 0);
  assertEqual(sf2.compareTo(sf1), 0);
  assertEqual(sf2.compareTo(sf2), 0);
  assertEqual(sf2.compareTo(ff1), 0);
  assertEqual(sf2.compareTo(ff2), 0);

  assertEqual(ff1.compareTo(ss1), 0);
  assertEqual(ff1.compareTo(ss2), 0);
  assertEqual(ff1.compareTo(fs1), 0);
  assertEqual(ff1.compareTo(fs2), 0);
  assertEqual(ff1.compareTo(sf1), 0);
  assertEqual(ff1.compareTo(sf2), 0);
  assertEqual(ff1.compareTo(ff1), 0);
  assertEqual(ff1.compareTo(ff2), 0);

  assertEqual(ff2.compareTo(ss1), 0);
  assertEqual(ff2.compareTo(ss2), 0);
  assertEqual(ff2.compareTo(fs1), 0);
  assertEqual(ff2.compareTo(fs2), 0);
  assertEqual(ff2.compareTo(sf1), 0);
  assertEqual(ff2.compareTo(sf2), 0);
  assertEqual(ff2.compareTo(ff1), 0);
  assertEqual(ff2.compareTo(ff2), 0);

  KString ss3("America/Bahia_Banderas", KEYWORDS, NUM_KEYWORDS);
  assertMore(ss1.compareTo(ss3), 0);
  assertMore(ss2.compareTo(ss3), 0);
  assertMore(fs1.compareTo(ss3), 0);
  assertMore(fs2.compareTo(ss3), 0);
  assertMore(sf1.compareTo(ss3), 0);
  assertMore(sf2.compareTo(ss3), 0);
  assertMore(ff1.compareTo(ss3), 0);
  assertMore(ff2.compareTo(ss3), 0);

  KString ss4("\x02" "Bahia_Banderas", KEYWORDS, NUM_KEYWORDS);
  assertMore(ss1.compareTo(ss4), 0);
  assertMore(ss2.compareTo(ss4), 0);
  assertMore(fs1.compareTo(ss4), 0);
  assertMore(fs2.compareTo(ss4), 0);
  assertMore(sf1.compareTo(ss4), 0);
  assertMore(sf2.compareTo(ss4), 0);
  assertMore(ff1.compareTo(ss4), 0);
  assertMore(ff2.compareTo(ss4), 0);
}

// ---------------------------------------------------------------------------
// printTo()
// ---------------------------------------------------------------------------

test(KStringTest, printTo_nullptr) {
  PrintStr<20> p;
  KString ss((const char*) nullptr, KEYWORDS, NUM_KEYWORDS);
  KString fs((const __FlashStringHelper*) nullptr, KEYWORDS, NUM_KEYWORDS);
  KString sf((const char*) nullptr, FLASHWORDS, NUM_KEYWORDS);
  KString ff((const __FlashStringHelper*) nullptr, FLASHWORDS, NUM_KEYWORDS);

  p.flush();
  ss.printTo(p);
  assertEqual(p.length(), (size_t) 0);

  p.flush();
  fs.printTo(p);
  assertEqual(p.length(), (size_t) 0);

  p.flush();
  sf.printTo(p);
  assertEqual(p.length(), (size_t) 0);

  p.flush();
  ff.printTo(p);
  assertEqual(p.length(), (size_t) 0);
}

test(KStringTest, printTo_empty) {
  PrintStr<20> p;
  KString ss("", KEYWORDS, NUM_KEYWORDS);
  KString fs(F(""), KEYWORDS, NUM_KEYWORDS);
  KString sf("", FLASHWORDS, NUM_KEYWORDS);
  KString ff(F(""), FLASHWORDS, NUM_KEYWORDS);

  p.flush();
  ss.printTo(p);
  assertEqual(p.length(), (size_t) 0);

  p.flush();
  fs.printTo(p);
  assertEqual(p.length(), (size_t) 0);

  p.flush();
  sf.printTo(p);
  assertEqual(p.length(), (size_t) 0);

  p.flush();
  ff.printTo(p);
  assertEqual(p.length(), (size_t) 0);
}

test(KStringTest, printTo_prefix_suffix) {
  PrintStr<20> p;

  KString ss1("America/Africa/", KEYWORDS, NUM_KEYWORDS);
  KString ss2("\x02\x01", KEYWORDS, NUM_KEYWORDS);
  KString fs1(F("America/Africa/"), KEYWORDS, NUM_KEYWORDS);
  KString fs2(F("\x02\x01"), KEYWORDS, NUM_KEYWORDS);
  KString sf1("America/Africa/", FLASHWORDS, NUM_KEYWORDS);
  KString sf2("\x02\x01", FLASHWORDS, NUM_KEYWORDS);
  KString ff1(F("America/Africa/"), FLASHWORDS, NUM_KEYWORDS);
  KString ff2(F("\x02\x01"), FLASHWORDS, NUM_KEYWORDS);

  p.flush();
  ss1.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");

  p.flush();
  ss2.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");

  p.flush();
  fs1.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");

  p.flush();
  fs2.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");

  p.flush();
  sf1.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");

  p.flush();
  sf2.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");

  p.flush();
  ff1.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");

  p.flush();
  ff2.printTo(p);
  assertEqual(p.cstr(), "America/Africa/");
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
  KString k(F("Africa"), FLASHWORDS, NUM_KEYWORDS);
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
  KString kf(F("\x01Lagos"), FLASHWORDS, NUM_KEYWORDS);
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
