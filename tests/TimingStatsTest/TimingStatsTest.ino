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

  assertEqual(1, stats.getCount());
  assertEqual(1, stats.getCounter());
  assertEqual(10, stats.getMin());
  assertEqual(10, stats.getMax());
  assertEqual(10, stats.getAvg());
  assertEqual(10, stats.getExpDecayAvg());
}

test(TimingStatsTest, two) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);

  assertEqual(2, stats.getCount());
  assertEqual(2, stats.getCounter());
  assertEqual(10, stats.getMin());
  assertEqual(20, stats.getMax());
  assertEqual(15, stats.getAvg());
  assertEqual(15, stats.getExpDecayAvg());
}

test(TimingStatsTest, three) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);
  stats.update(30);

  assertEqual(3, stats.getCount());
  assertEqual(3, stats.getCounter());
  assertEqual(10, stats.getMin());
  assertEqual(30, stats.getMax());
  assertEqual(20, stats.getAvg());
  assertEqual(22, stats.getExpDecayAvg());
}

test(TimingStatsTest, four) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);
  stats.update(30);
  stats.update(40);

  assertEqual(4, stats.getCount());
  assertEqual(4, stats.getCounter());
  assertEqual(10, stats.getMin());
  assertEqual(40, stats.getMax());
  assertEqual(25, stats.getAvg());
  assertEqual(31, stats.getExpDecayAvg());
}

test(TimingStatsTest, reset) {
  TimingStats stats;
  stats.update(10);
  stats.update(20);
  stats.update(30);
  stats.update(40);

  assertEqual(4, stats.getCount());
  assertEqual(4, stats.getCounter());

  stats.reset();
  assertEqual(0, stats.getCount());
  assertEqual(4, stats.getCounter());
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
