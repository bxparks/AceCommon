#line 2 "PrintPadTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

test(PrintPadTest, printPad2To) {
  PrintStr<10> str;

  printPad2To(str, 2, '0');
  assertEqual("02", str.getCstr());

  str.flush();
  printPad2To(str, 22, '0');
  assertEqual("22", str.getCstr());
}

test(PrintPadTest, printPad3To) {
  PrintStr<10> str;

  printPad3To(str, 2, '0');
  assertEqual("002", str.getCstr());

  str.flush();
  printPad3To(str, 22, '0');
  assertEqual("022", str.getCstr());

  str.flush();
  printPad3To(str, 222, '0');
  assertEqual("222", str.getCstr());
}

test(PrintPadTest, printPad4To) {
  PrintStr<10> str;

  printPad4To(str, 2, '0');
  assertEqual("0002", str.getCstr());

  str.flush();
  printPad4To(str, 22, '0');
  assertEqual("0022", str.getCstr());

  str.flush();
  printPad4To(str, 222, '0');
  assertEqual("0222", str.getCstr());

  str.flush();
  printPad4To(str, 2222, '0');
  assertEqual("2222", str.getCstr());
}

test(PrintPadTest, printPad5To) {
  PrintStr<10> str;

  printPad5To(str, 2, '0');
  assertEqual("00002", str.getCstr());

  str.flush();
  printPad5To(str, 22, '0');
  assertEqual("00022", str.getCstr());

  str.flush();
  printPad5To(str, 222, '0');
  assertEqual("00222", str.getCstr());

  str.flush();
  printPad5To(str, 2222, '0');
  assertEqual("02222", str.getCstr());

  str.flush();
  printPad5To(str, 22222, '0');
  assertEqual("22222", str.getCstr());
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
#ifndef UNIX_HOST_DUINO
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while(!SERIAL_PORT_MONITOR); // for the Arduino Leonardo/Micro only
}

void loop() {
  aunit::TestRunner::run();
}
