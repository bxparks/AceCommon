#line 2 "PrintReplaceTestTo.ino"

#include <AUnitVerbose.h>
#include <AceCommon.h>

using aunit::TestRunner;
using ace_common::printReplaceCharTo;
using ace_common::printReplaceStringTo;
using ace_common::PrintStr;

//----------------------------------------------------------------------------
// printReplaceCharTo(char*)
//----------------------------------------------------------------------------

test(printReplaceCharTo, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, "NOREPLACE", '%', 's');
  assertEqual("NOREPLACE", printStr.getCstr());

  printStr.flush();
  printReplaceCharTo(printStr, "E%T%Z", '%', 's');
  assertEqual("EsTsZ", printStr.getCstr());

  printStr.flush();
  printReplaceCharTo(printStr, "E%T%Z", '%', '\0');
  assertEqual("ETZ", printStr.getCstr());
}

test(printReplaceCharTo, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, "E%T", '%', 's');
  assertEqual("Es", printStr.getCstr());
}

//----------------------------------------------------------------------------
// printReplaceCharTo(F())
//----------------------------------------------------------------------------

test(printReplaceCharTo_Flash, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, F("NOREPLACE"), '%', 's');
  assertEqual("NOREPLACE", printStr.getCstr());

  printStr.flush();
  printReplaceCharTo(printStr, F("E%T%Z"), '%', 's');
  assertEqual("EsTsZ", printStr.getCstr());

  printStr.flush();
  printReplaceCharTo(printStr, F("E%T%Z"), '%', '\0');
  assertEqual("ETZ", printStr.getCstr());
}

test(printReplaceCharTo_Flash, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, F("E%T"), '%', 's');
  assertEqual("Es", printStr.getCstr());
}

//---------------------------------------------------------------------------
// printReplaceStringTo(char*)
//---------------------------------------------------------------------------

test(printReplaceStringTo, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, "NOREPLACE", '%', "suv");
  assertEqual("NOREPLACE", printStr.getCstr());

  printStr.flush();
  printReplaceStringTo(printStr, "E%T%Z", '%', "suv");
  assertEqual("EsuvTsuvZ", printStr.getCstr());

  printStr.flush();
  printReplaceStringTo(printStr, "E%T%Z", '%', "");
  assertEqual("ETZ", printStr.getCstr());
}

test(printReplaceStringTo, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, "E%T", '%', "suv");
  assertEqual("Es", printStr.getCstr());
}

//---------------------------------------------------------------------------
// printReplaceStringTo(F())
//---------------------------------------------------------------------------

test(printReplaceStringTo_Flash, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, F("NOREPLACE"), '%', "suv");
  assertEqual("NOREPLACE", printStr.getCstr());

  printStr.flush();
  printReplaceStringTo(printStr, F("E%T%Z"), '%', "suv");
  assertEqual("EsuvTsuvZ", printStr.getCstr());

  printStr.flush();
  printReplaceStringTo(printStr, F("E%T%Z"), '%', "");
  assertEqual("ETZ", printStr.getCstr());
}

test(printReplaceStringTo_Flash, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, F("E%T"), '%', "suv");
  assertEqual("Es", printStr.getCstr());
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
