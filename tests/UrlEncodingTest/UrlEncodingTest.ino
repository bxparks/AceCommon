#line 2 "UrlEncodingTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

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
}

void loop() {
  aunit::TestRunner::run();
}
