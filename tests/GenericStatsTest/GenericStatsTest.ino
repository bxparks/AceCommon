#line 2 "GenericStatsTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using aunit::TestRunner;
using ace_common::GenericStats;

//----------------------------------------------------------------------------
// Test GenericStats
//----------------------------------------------------------------------------

test(GenericStatsTest, one) {
  GenericStats<float> stats;
  stats.update(10.0);

  assertEqual((uint16_t) 1, stats.getCount());
  assertEqual((uint16_t) 1, stats.getCounter());
  assertEqual(10.0, stats.getMin());
  assertEqual(10.0, stats.getMax());
  assertEqual(10.0, stats.getAvg());
  assertEqual(10.0, stats.getExpDecayAvg());
}

test(GenericStatsTest, two) {
  GenericStats<float> stats;
  stats.update(10.0);
  stats.update(20.0);

  assertEqual((uint16_t) 2, stats.getCount());
  assertEqual((uint16_t) 2, stats.getCounter());
  assertEqual(10.0, stats.getMin());
  assertEqual(20.0, stats.getMax());
  assertEqual(15.0, stats.getAvg());
  assertEqual(15.0, stats.getExpDecayAvg());
}

test(GenericStatsTest, three) {
  GenericStats<float> stats;
  stats.update(10.0);
  stats.update(20.0);
  stats.update(30.0);

  assertEqual(3, stats.getCount());
  assertEqual(3, stats.getCounter());
  assertEqual(10.0, stats.getMin());
  assertEqual(30.0, stats.getMax());
  assertEqual(20.0, stats.getAvg());
  assertEqual(22.5, stats.getExpDecayAvg());
}

test(GenericStatsTest, four) {
  GenericStats<float> stats;
  stats.update(10.0);
  stats.update(20.0);
  stats.update(30.0);
  stats.update(40.0);

  assertEqual((uint16_t) 4, stats.getCount());
  assertEqual((uint16_t) 4, stats.getCounter());
  assertEqual(10.0, stats.getMin());
  assertEqual(40.0, stats.getMax());
  assertEqual(25.0, stats.getAvg());
  assertEqual(31.25, stats.getExpDecayAvg());
}

test(GenericStatsTest, expDecayAvg_doesNotOverflow) {
  GenericStats<float> stats;
  stats.update(60000.0);
  stats.update(60001.0);
  assertEqual(60000.5, stats.getExpDecayAvg());

  stats.reset();
  stats.update(60000.0);
  stats.update(60002.0);
  assertEqual(60001.0, stats.getExpDecayAvg());
}

test(GenericStatsTest, reset) {
  GenericStats<float> stats;
  stats.update(10.0);
  stats.update(20.0);
  stats.update(30.0);
  stats.update(40.0);

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
#ifndef EPOXY_DUINO
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while(!SERIAL_PORT_MONITOR); // for the Arduino Leonardo/Micro only
}

void loop() {
  aunit::TestRunner::run();
}
