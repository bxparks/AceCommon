#line 2 "CopyReplaceTest.ino"

#include <AceCommon.h>
#include <AUnitVerbose.h>

using aunit::TestRunner;
using ace_common::copyReplaceChar;
using ace_common::copyReplaceString;

//----------------------------------------------------------------------------
// copyReplaceChar(char*)
//----------------------------------------------------------------------------

test(copyReplaceChar, normal_char) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceChar(dst, dstSize, "NOREPLACE", '%', 's');
  assertEqual("NOREPLACE", dst);

  copyReplaceChar(dst, dstSize, "E%T%Z", '%', 's');
  assertEqual("EsTsZ", dst);

  copyReplaceChar(dst, dstSize, "E%T", '%', '\0');
  assertEqual("ET", dst);
}

test(copyReplaceChar, outOfBounds_char) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceChar(dst, dstSize, "E%T%Z", '%', 's');
  assertEqual("Es", dst);
}

//----------------------------------------------------------------------------
// copyReplaceChar(FlashString)
//----------------------------------------------------------------------------

test(copyReplaceChar, normal_flash) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceChar(dst, dstSize, F("NOREPLACE"), '%', 's');
  assertEqual("NOREPLACE", dst);

  copyReplaceChar(dst, dstSize, F("E%T%Z"), '%', 's');
  assertEqual("EsTsZ", dst);

  copyReplaceChar(dst, dstSize, F("E%T"), '%', '\0');
  assertEqual("ET", dst);
}

test(copyReplaceChar, outOfBounds_flash) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceChar(dst, dstSize, F("E%T%Z"), '%', 's');
  assertEqual("Es", dst);
}

//---------------------------------------------------------------------------
// copyReplaceString(char*, char*)
//---------------------------------------------------------------------------

test(copyReplaceString, normal_char_char) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, "NOREPLACE", '%', "suv");
  assertEqual("NOREPLACE", dst);

  copyReplaceString(dst, dstSize, "E%T%Z", '%', "suv");
  assertEqual("EsuvTsuvZ", dst);

  copyReplaceString(dst, dstSize, "E%T", '%', "");
  assertEqual("ET", dst);
}

test(copyReplaceString, outOfBounds_char_char) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, "E%T%Z", '%', "suv");
  assertEqual("Es", dst);
}

//---------------------------------------------------------------------------
// copyReplaceString(char*, FlashString)
//---------------------------------------------------------------------------

test(copyReplaceString, normal_char_flash) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, "NOREPLACE", '%', F("suv"));
  assertEqual("NOREPLACE", dst);

  copyReplaceString(dst, dstSize, "E%T%Z", '%', F("suv"));
  assertEqual("EsuvTsuvZ", dst);

  copyReplaceString(dst, dstSize, "E%T", '%', F(""));
  assertEqual("ET", dst);
}

test(copyReplaceString, outOfBounds_char_flash) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, "E%T%Z", '%', F("suv"));
  assertEqual("Es", dst);
}

//---------------------------------------------------------------------------
// copyReplaceString(FlashString, char*)
//---------------------------------------------------------------------------

test(copyReplaceString, normal_flash_char) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, F("NOREPLACE"), '%', "suv");
  assertEqual("NOREPLACE", dst);

  copyReplaceString(dst, dstSize, F("E%T%Z"), '%', "suv");
  assertEqual("EsuvTsuvZ", dst);

  copyReplaceString(dst, dstSize, F("E%T"), '%', "");
  assertEqual("ET", dst);
}

test(copyReplaceString, outOfBounds_flash_char) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, F("E%T%Z"), '%', "suv");
  assertEqual("Es", dst);
}

//---------------------------------------------------------------------------
// copyReplaceString(FlashString, FlashString)
//---------------------------------------------------------------------------

test(copyReplaceString, normal_flash_flash) {
  const uint8_t dstSize = 20;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, F("NOREPLACE"), '%', F("suv"));
  assertEqual("NOREPLACE", dst);

  copyReplaceString(dst, dstSize, F("E%T%Z"), '%', F("suv"));
  assertEqual("EsuvTsuvZ", dst);

  copyReplaceString(dst, dstSize, F("E%T"), '%', F(""));
  assertEqual("ET", dst);
}

test(copyReplaceString, outOfBounds_flash_flash) {
  const uint8_t dstSize = 3;
  char dst[dstSize];

  copyReplaceString(dst, dstSize, F("E%T%Z"), '%', F("suv"));
  assertEqual("Es", dst);
}

// ---------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // some boards reboot twice
#endif

  Serial.begin(115200);
  while (!Serial); // Leonardo/Micro

#if defined(EPOXY_DUINO)
  Serial.setLineModeUnix();
#endif
}

void loop() {
  TestRunner::run();
}
