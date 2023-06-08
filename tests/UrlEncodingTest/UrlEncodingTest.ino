#line 2 "UrlEncodingTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

//----------------------------------------------------------------------------
// Test byteToHexChar() and hexCharToByte()
//----------------------------------------------------------------------------

test(formUrlEncode, byteToHexChar_upperCase) {
  char high, low;

  byteToHexChar(0x01, &high, &low);
  assertEqual('0', high);
  assertEqual('1', low);

  byteToHexChar(0x9a, &high, &low);
  assertEqual('9', high);
  assertEqual('A', low);

  byteToHexChar(0xef, &high, &low);
  assertEqual('E', high);
  assertEqual('F', low);
}

test(formUrlEncode, byteToHexChar_lowerCase) {
  char high, low;

  byteToHexChar(0x01, &high, &low, 'a');
  assertEqual('0', high);
  assertEqual('1', low);

  byteToHexChar(0x9a, &high, &low, 'a');
  assertEqual('9', high);
  assertEqual('a', low);

  byteToHexChar(0xef, &high, &low, 'a');
  assertEqual('e', high);
  assertEqual('f', low);
}

test(formUrlEncode, hexCharToByte) {
  assertEqual(0, hexCharToByte('0'));
  assertEqual(9, hexCharToByte('9'));
  assertEqual(10, hexCharToByte('a'));
  assertEqual(15, hexCharToByte('f'));
  assertEqual(0, hexCharToByte('g'));
}

//----------------------------------------------------------------------------
// Test formUrlEncode()
//----------------------------------------------------------------------------

test(formUrlEncode, normal) {
  PrintStr<10> printStr;
  formUrlEncode(printStr, "0aA %");
  assertEqual("0aA+%25", printStr.cstr());
}

test(formUrlEncode, tooLong) {
  PrintStr<10> printStr;
  // truncated to 9 characters because PrintStr is only 10-characters big
  formUrlEncode(printStr, "0aA %0123456");
  assertEqual("0aA+%2501", printStr.cstr());
}

//----------------------------------------------------------------------------
// Test formUrlDecode()
//----------------------------------------------------------------------------

test(formUrlDecode, normal) {
  PrintStr<10> printStr;
  formUrlDecode(printStr, "0aA+%25");
  assertEqual("0aA %", printStr.cstr());
}

test(formUrlDecode, truncated1) {
  PrintStr<10> printStr;
  // Missing 2-digit of hex after %, which is ignored.
  formUrlDecode(printStr, "0aA+%2");
  assertEqual("0aA ", printStr.cstr());
}

test(formUrlDecode, truncated2) {
  PrintStr<10> printStr;
  // Missing 2-digit of hex after %, which is ignored.
  formUrlDecode(printStr, "0aA+%");
  assertEqual("0aA ", printStr.cstr());
}

test(formUrlDecode, invalidHex) {
  PrintStr<10> printStr;
  // Missing 2-digit of hex after %, which is ignored.
  formUrlDecode(printStr, "0aA+%0Z");
  assertEqual("0aA ", printStr.cstr());
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

#if defined(EPOXY_DUINO)
  Serial.setLineModeUnix();
#endif
}

void loop() {
  aunit::TestRunner::run();
}
