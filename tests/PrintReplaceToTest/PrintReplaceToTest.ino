#line 2 "PrintReplaceTestTo.ino"

#include <AUnitVerbose.h>
#include <AceCommon.h>

using aunit::TestRunner;
using ace_common::printReplaceCharTo;
using ace_common::printReplaceStringTo;
using ace_common::PrintStr;

//----------------------------------------------------------------------------
// printReplaceCharTo()
//----------------------------------------------------------------------------

test(printReplaceCharTo, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, "NOREPLACE", '%', 'S');
  assertEqual("NOREPLACE", printStr.getCstr());

  printStr.flush();
  printReplaceCharTo(printStr, "E%T", '%', 'S');
  assertEqual("EST", printStr.getCstr());

  printStr.flush();
  printReplaceCharTo(printStr, "E%T", '%', '\0');
  assertEqual("ET", printStr.getCstr());
}

test(printReplaceCharTo, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, "E%T", '%', 'S');
  assertEqual("ES", printStr.getCstr());
}

//---------------------------------------------------------------------------
// printReplaceStringTo()
//---------------------------------------------------------------------------

test(printReplaceStringTo, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, "NOREPLACE", '%', "SUV");
  assertEqual("NOREPLACE", printStr.getCstr());

  printStr.flush();
  printReplaceStringTo(printStr, "E%T", '%', "SUV");
  assertEqual("ESUVT", printStr.getCstr());

  printStr.flush();
  printReplaceStringTo(printStr, "E%T", '%', "");
  assertEqual("ET", printStr.getCstr());
}

test(printReplaceStringTo, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, "E%T", '%', "SUV");
  assertEqual("ES", printStr.getCstr());
}

// ---------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // some boards reboot twice
#endif

  Serial.begin(115200);
  while (!Serial); // Leonardo/Micro
}

void loop() {
  TestRunner::run();
}
