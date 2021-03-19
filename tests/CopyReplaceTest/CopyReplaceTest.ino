#line 2 "CopyReplaceTest.ino"

#include <AceCommon.h>
#include <AUnitVerbose.h>

using aunit::TestRunner;
using ace_common::copyReplaceChar;
using ace_common::copyReplaceString;

//----------------------------------------------------------------------------
// copyReplaceChar()
//----------------------------------------------------------------------------

test(copyReplaceChar, normal) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceChar(dst, dstSize, "NOREPLACE", '%', 's');
  assertEqual("NOREPLACE", dst);

  copyReplaceChar(dst, dstSize, "E%T%Z", '%', 's');
  assertEqual("EsTsZ", dst);

  copyReplaceChar(dst, dstSize, "E%T", '%', '\0');
  assertEqual("ET", dst);
}

test(copyReplaceChar, outOfBounds) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceChar(dst, dstSize, "E%T%Z", '%', 's');
  assertEqual("Es", dst);
}

//---------------------------------------------------------------------------
// copyReplaceString()
//---------------------------------------------------------------------------

test(copyReplaceString, normal) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, "NOREPLACE", '%', "suv");
  assertEqual("NOREPLACE", dst);

  copyReplaceString(dst, dstSize, "E%T%Z", '%', "suv");
  assertEqual("EsuvTsuvZ", dst);

  copyReplaceString(dst, dstSize, "E%T", '%', "");
  assertEqual("ET", dst);
}

test(copyReplaceString, outOfBounds) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, "E%T%Z", '%', "suv");
  assertEqual("Es", dst);
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
