#line 2 "PStringsTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

// --------------------------------------------------------------------------
// strcmp_PP()
// --------------------------------------------------------------------------

test(strcmp_PP, nullptr) {
  assertEqual(strcmp_PP(nullptr, nullptr), 0);
  assertMore(strcmp_PP("", nullptr), 0);
  assertLess(strcmp_PP(nullptr, ""), 0);
}

test(strcmp_PP, not_null) {
  assertEqual(strcmp_PP("", ""), 0);
  assertEqual(strcmp_PP("a", "a"), 0);
  assertMore(strcmp_PP("ab", "a"), 0);
  assertLess(strcmp_PP("a", "ab"), 0);
}

// --------------------------------------------------------------------------

void setup() {
#if ! defined(UNIX_HOST_DUINO)
  delay(1000); // wait to prevent garbage on SERIAL_PORT_MONITOR
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while(!SERIAL_PORT_MONITOR); // Arduino Leonardo/Micro only
}

void loop() {
  TestRunner::run();
}
