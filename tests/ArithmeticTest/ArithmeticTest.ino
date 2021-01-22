#line 2 "ArithmeticTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

// --------------------------------------------------------------------------
// incrementMod()
// --------------------------------------------------------------------------

test(incrementMod) {
  const int PERIOD = 3;
  int counter = 0;

  incrementMod(counter, PERIOD);
  assertEqual(1, counter);

  incrementMod(counter, PERIOD);
  assertEqual(2, counter);

  incrementMod(counter, PERIOD);
  assertEqual(0, counter);
}

test(incrementModOffset) {
  const int PERIOD = 12;
  const int OFFSET = 1;
  int month = 10;

  incrementModOffset(month, PERIOD, OFFSET);
  assertEqual(11, month);

  incrementModOffset(month, PERIOD, OFFSET);
  assertEqual(12, month);

  incrementModOffset(month, PERIOD, OFFSET);
  assertEqual(1, month);
}

// --------------------------------------------------------------------------
// decToBcd(), bcdToDec()
// --------------------------------------------------------------------------

test(decToBcd) {
  assertEqual(0x00, decToBcd(0));
  assertEqual(0x10, decToBcd(10));
  assertEqual(0x23, decToBcd(23));
  assertEqual(0x99, decToBcd(99));
}

test(bcdToDec) {
  assertEqual(0, bcdToDec(0x00));
  assertEqual(10, bcdToDec(0x10));
  assertEqual(23, bcdToDec(0x23));
  assertEqual(99, bcdToDec(0x99));
}

// --------------------------------------------------------------------------
// udiv1000()
// --------------------------------------------------------------------------

test(udiv1000) {
  assertEqual(9UL, udiv1000(10L*1000));
  assertEqual(97UL, udiv1000(100L*1000));
  assertEqual(997UL, udiv1000(1000L*1000));
  assertEqual(9993UL, udiv1000(10L*1000*1000));
  assertEqual(99987UL, udiv1000(100L*1000*1000));
  assertEqual(999980UL, udiv1000(1000L*1000*1000));
}

// --------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // wait to prevent garbage on SERIAL_PORT_MONITOR
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while(!SERIAL_PORT_MONITOR); // Arduino Leonardo/Micro only
}

void loop() {
  TestRunner::run();
}
