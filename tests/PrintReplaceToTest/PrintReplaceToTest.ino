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
  assertEqual("NOREPLACE", printStr.cstr());

  printStr.flush();
  printReplaceCharTo(printStr, "E%T%Z", '%', 's');
  assertEqual("EsTsZ", printStr.cstr());

  printStr.flush();
  printReplaceCharTo(printStr, "E%T%Z", '%', '\0');
  assertEqual("ETZ", printStr.cstr());
}

test(printReplaceCharTo, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, "E%T", '%', 's');
  assertEqual("Es", printStr.cstr());
}

//----------------------------------------------------------------------------
// printReplaceCharTo(F())
//----------------------------------------------------------------------------

test(printReplaceCharTo_Flash, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, F("NOREPLACE"), '%', 's');
  assertEqual("NOREPLACE", printStr.cstr());

  printStr.flush();
  printReplaceCharTo(printStr, F("E%T%Z"), '%', 's');
  assertEqual("EsTsZ", printStr.cstr());

  printStr.flush();
  printReplaceCharTo(printStr, F("E%T%Z"), '%', '\0');
  assertEqual("ETZ", printStr.cstr());
}

test(printReplaceCharTo_Flash, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceCharTo(printStr, F("E%T"), '%', 's');
  assertEqual("Es", printStr.cstr());
}

//---------------------------------------------------------------------------
// printReplaceStringTo(char*)
//---------------------------------------------------------------------------

test(printReplaceStringTo, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, "NOREPLACE", '%', "suv");
  assertEqual("NOREPLACE", printStr.cstr());

  printStr.flush();
  printReplaceStringTo(printStr, "E%T%Z", '%', "suv");
  assertEqual("EsuvTsuvZ", printStr.cstr());

  printStr.flush();
  printReplaceStringTo(printStr, "E%T%Z", '%', "");
  assertEqual("ETZ", printStr.cstr());
}

test(printReplaceStringTo, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, "E%T", '%', "suv");
  assertEqual("Es", printStr.cstr());
}

//---------------------------------------------------------------------------
// printReplaceStringTo(F())
//---------------------------------------------------------------------------

test(printReplaceStringTo_Flash, normal) {
  PrintStr<20> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, F("NOREPLACE"), '%', "suv");
  assertEqual("NOREPLACE", printStr.cstr());

  printStr.flush();
  printReplaceStringTo(printStr, F("E%T%Z"), '%', "suv");
  assertEqual("EsuvTsuvZ", printStr.cstr());

  printStr.flush();
  printReplaceStringTo(printStr, F("E%T%Z"), '%', "");
  assertEqual("ETZ", printStr.cstr());
}

test(printReplaceStringTo_Flash, outOfBounds) {
  PrintStr<3> printStr;

  printStr.flush();
  printReplaceStringTo(printStr, F("E%T"), '%', "suv");
  assertEqual("Es", printStr.cstr());
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
