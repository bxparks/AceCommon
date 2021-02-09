#line 2 "HashTest.ino"

#include <Arduino.h>
#include <AUnit.h>
#include <AceCommon.h>

using aunit::TestRunner;
using ace_common::hashDjb2;

test(hashDjb2, normal_string) {
  assertEqual((uint32_t) 5381, hashDjb2(""));
  assertEqual((uint32_t) 177670, hashDjb2("a"));
  assertEqual((uint32_t) 177671, hashDjb2("b"));
  assertEqual((uint32_t) 5863208, hashDjb2("ab"));
  assertEqual((uint32_t) 193485963, hashDjb2("abc"));
  assertEqual((uint32_t) 2090069583, hashDjb2("abcd"));
  assertEqual((uint32_t) 252819604, hashDjb2("abcde"));
}

test(hashDjb2, flash_string) {
  assertEqual((uint32_t) 5381, hashDjb2(F("")));
  assertEqual((uint32_t) 177670, hashDjb2(F("a")));
  assertEqual((uint32_t) 177671, hashDjb2(F("b")));
  assertEqual((uint32_t) 5863208, hashDjb2(F("ab")));
  assertEqual((uint32_t) 193485963, hashDjb2(F("abc")));
  assertEqual((uint32_t) 2090069583, hashDjb2(F("abcd")));
  assertEqual((uint32_t) 252819604, hashDjb2(F("abcde")));
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
