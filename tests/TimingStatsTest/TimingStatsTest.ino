#line 2 "TimingStatsTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

//----------------------------------------------------------------------------
// Test TimingStats
//----------------------------------------------------------------------------

test(TimingStatsTest, one) {
  TimingStats stats;
  stats.update(10);

  assertEqual((uint16_t) 1, stats.getCount());
  assertEqual((uint16_t) 1, stats.getCounter());
  assertEqual((uint16_t) 10, stats.getMin());
  assertEqual((uint16_t) 10, stats.getMax());
  assertEqual((uint16_t) 10, stats.getAvg());
  assertEqual((uint16_t) 10, stats.getExpDecayAvg());
}

test(TimingStatsTest, two) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);

  assertEqual((uint16_t) 2, stats.getCount());
  assertEqual((uint16_t) 2, stats.getCounter());
  assertEqual((uint16_t) 10, stats.getMin());
  assertEqual((uint16_t) 20, stats.getMax());
  assertEqual((uint16_t) 15, stats.getAvg());
  assertEqual((uint16_t) 15, stats.getExpDecayAvg());
}

test(TimingStatsTest, three) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);
  stats.update(30);

  assertEqual((uint16_t) 3, stats.getCount());
  assertEqual((uint16_t) 3, stats.getCounter());
  assertEqual((uint16_t) 10, stats.getMin());
  assertEqual((uint16_t) 30, stats.getMax());
  assertEqual((uint16_t) 20, stats.getAvg());
  assertEqual((uint16_t) 22, stats.getExpDecayAvg());
}

test(TimingStatsTest, four) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);
  stats.update(30);
  stats.update(40);

  assertEqual((uint16_t) 4, stats.getCount());
  assertEqual((uint16_t) 4, stats.getCounter());
  assertEqual((uint16_t) 10, stats.getMin());
  assertEqual((uint16_t) 40, stats.getMax());
  assertEqual((uint16_t) 25, stats.getAvg());
  assertEqual((uint16_t) 31, stats.getExpDecayAvg());
}

test(TimingStatsTest, reset) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);
  stats.update(30);
  stats.update(40);

  assertEqual((uint16_t) 4, stats.getCount());
  assertEqual((uint16_t) 4, stats.getCounter());

  stats.reset();
  assertEqual((uint16_t) 0, stats.getCount());
  assertEqual((uint16_t) 4, stats.getCounter());
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
