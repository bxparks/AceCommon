#line 2 "PrintIntAsFloat.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

// ESP32 does not define SERIAL_PORT_MONITOR
#ifndef SERIAL_PORT_MONITOR
  #define SERIAL_PORT_MONITOR Serial
#endif

test(PrintUint16AsFloat3) {
  PrintStr<16> printStr;

  printStr.flush();
  printUint16AsFloat3To(printStr, 0U);
  assertEqual("0.000", printStr.cstr());

  printStr.flush();
  printUint16AsFloat3To(printStr, 1U);
  assertEqual("0.001", printStr.cstr());

  printStr.flush();
  printUint16AsFloat3To(printStr, 12U);
  assertEqual("0.012", printStr.cstr());

  printStr.flush();
  printUint16AsFloat3To(printStr, 123U);
  assertEqual("0.123", printStr.cstr());

  printStr.flush();
  printUint16AsFloat3To(printStr, 1234U);
  assertEqual("1.234", printStr.cstr());

  printStr.flush();
  printUint16AsFloat3To(printStr, 12345U);
  assertEqual("12.345", printStr.cstr());
}

test(PrintUint32AsFloat3) {
  PrintStr<16> printStr;

  printStr.flush();
  printUint32AsFloat3To(printStr, 0UL);
  assertEqual("0.000", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 1UL);
  assertEqual("0.001", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 12UL);
  assertEqual("0.012", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 123UL);
  assertEqual("0.123", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 1234UL);
  assertEqual("1.234", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 12345UL);
  assertEqual("12.345", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 123456UL);
  assertEqual("123.456", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 1234567UL);
  assertEqual("1234.567", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 12345678UL);
  assertEqual("12345.678", printStr.cstr());

  printStr.flush();
  printUint32AsFloat3To(printStr, 123456789UL);
  assertEqual("123456.789", printStr.cstr());
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
