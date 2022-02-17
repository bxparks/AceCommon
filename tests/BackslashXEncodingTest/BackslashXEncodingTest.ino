#line 2 "BackslashXEncodingTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

//----------------------------------------------------------------------------
// Test isHexChar()
//----------------------------------------------------------------------------

test(isHexChar) {
  assertTrue(isHexChar('0'));
  assertTrue(isHexChar('9'));
  assertTrue(isHexChar('a'));
  assertTrue(isHexChar('f'));
  assertTrue(isHexChar('A'));
  assertTrue(isHexChar('F'));

  assertFalse(isHexChar('g'));
  assertFalse(isHexChar('z'));
  assertFalse(isHexChar(' '));
  assertFalse(isHexChar('.'));
}

//----------------------------------------------------------------------------
// Test backslashXEncode()
//----------------------------------------------------------------------------

test(backslashXEncode, normal) {
  char t[64];
  uint8_t status;

  size_t written = backslashXEncode(t, sizeof(t), "\nab\\cd\r", &status);
  assertEqual(written, (size_t) 14);
  assertEqual(status, 0);
  assertEqual(R"(\x0Aab\\cd\x0D)", t);
}

test(backslashXEncode, tooLong) {
  char t[8];
  uint8_t status;

  size_t written = backslashXEncode(t, sizeof(t), "\nab\\cd\r", &status);
  assertEqual(written, (size_t) 7);
  assertEqual(status, 1);
  assertEqual(R"(\x0Aab\)", t);
}

//----------------------------------------------------------------------------
// Test backslashXDecode()
//----------------------------------------------------------------------------

test(backslashXDecode, normal) {
  char t[64];
  uint8_t status;

  size_t written = backslashXDecode(
      t, sizeof(t), R"(\x0Aab\\cd\x0D\xFF)", &status);
  assertEqual(written, (size_t) 8);
  assertEqual(status, 0);
  assertEqual("\x0A" "ab\\cd" "\x0D" "\xFF", t);
}

test(backslashXDecode, tooLong) {
  char t[6];
  uint8_t status;

  size_t written = backslashXDecode(t, sizeof(t), R"(\x0Aab\\cd\x0D)", &status);
  assertEqual(written, (size_t) 5);
  assertEqual(status, 1);
  assertEqual("\x0A" "ab\\c", t);
}

test(backslashXDecode, illformed) {
  char t[64];
  uint8_t status;

  // '\' should be followed something
  size_t written = backslashXDecode(t, sizeof(t), R"(a\)", &status);
  assertEqual(status, 1);
  assertEqual(written, (size_t) 1);

  // '\' should be followed by 'x' or another '\'
  written = backslashXDecode(t, sizeof(t), R"(a\a)", &status);
  assertEqual(status, 1);
  assertEqual(written, (size_t) 1);

  // '\xhh' should be followed by 2 digits
  written = backslashXDecode(t, sizeof(t), R"(a\xA)", &status);
  assertEqual(status, 1);
  assertEqual(written, (size_t) 1);

  // '\xhh' should be followed by 2 hexadecimal digits
  written = backslashXDecode(t, sizeof(t), R"(a\xAG)", &status);
  assertEqual(status, 1);
  assertEqual(written, (size_t) 1);

  // input should not contain unprintable characters
  written = backslashXDecode(t, sizeof(t), "a\r", &status);
  assertEqual(status, 1);
  assertEqual(written, (size_t) 1);
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
#ifndef EPOXY_DUINO
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while(!SERIAL_PORT_MONITOR); // for the Arduino Leonardo/Micro only
}

void loop() {
  aunit::TestRunner::run();
}
