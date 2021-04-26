#line 2 "PrintPadTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

//----------------------------------------------------------------------------

test(PrintfToTest, printfTo) {
  PrintStr<10> str;

  printfTo(str, "%d", 10);
  assertEqual("10", str.cstr());

  str.flush();
  printfTo(str, "%x", 10);
  assertEqual("a", str.cstr());

  str.flush();
  printfTo(str, "%03d", 10);
  assertEqual("010", str.cstr());

  str.flush();
  printfTo(str, "%s", "hello");
  assertEqual("hello", str.cstr());
}

//----------------------------------------------------------------------------

test(PrintPadTest, printPad2To) {
  PrintStr<10> str;

  printPad2To(str, 2, '0');
  assertEqual("02", str.cstr());

  str.flush();
  printPad2To(str, 22, '0');
  assertEqual("22", str.cstr());
}

test(PrintPadTest, printPad3To) {
  PrintStr<10> str;

  printPad3To(str, 2, '0');
  assertEqual("002", str.cstr());

  str.flush();
  printPad3To(str, 22, '0');
  assertEqual("022", str.cstr());

  str.flush();
  printPad3To(str, 222, '0');
  assertEqual("222", str.cstr());
}

test(PrintPadTest, printPad4To) {
  PrintStr<10> str;

  printPad4To(str, 2, '0');
  assertEqual("0002", str.cstr());

  str.flush();
  printPad4To(str, 22, '0');
  assertEqual("0022", str.cstr());

  str.flush();
  printPad4To(str, 222, '0');
  assertEqual("0222", str.cstr());

  str.flush();
  printPad4To(str, 2222, '0');
  assertEqual("2222", str.cstr());
}

test(PrintPadTest, printPad5To) {
  PrintStr<10> str;

  printPad5To(str, 2, '0');
  assertEqual("00002", str.cstr());

  str.flush();
  printPad5To(str, 22, '0');
  assertEqual("00022", str.cstr());

  str.flush();
  printPad5To(str, 222, '0');
  assertEqual("00222", str.cstr());

  str.flush();
  printPad5To(str, 2222, '0');
  assertEqual("02222", str.cstr());

  str.flush();
  printPad5To(str, 22222, '0');
  assertEqual("22222", str.cstr());
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
