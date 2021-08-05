#line 2 "ReverseTest.ino"

#include <Arduino.h>
#include <AUnit.h>
#include <AceCommon.h>

using aunit::TestRunner;
using ace_common::reverse;
using ace_common::isSorted;

//-----------------------------------------------------------------------------

static int LIST0[] = {};

// Should do nothing (e.g. don't seg fault on Linux) for a zero-sized array.
test(reverse, list0) {
  reverse(LIST0, 0);
}

//-----------------------------------------------------------------------------

static int LIST1[] = { 1 };

// Should do nothing for a single item list.
test(reverse, list1) {
  reverse(LIST1, 1);
  assertEqual(1, LIST1[0]);
}

//-----------------------------------------------------------------------------

static int LIST2[] = { 1, 2 };

// Should reverse the 2-element array.
test(reverse, list2) {
  reverse(LIST2, 2);
  assertEqual(LIST2[0], 2);
  assertEqual(LIST2[1], 1);
}

//-----------------------------------------------------------------------------

static int LIST5[] = {
  5, 4, 3, 2, 1
};

test(reverse, list5) {
  assertFalse(isSorted(LIST5, 5));

  reverse(LIST5, 5);
  assertTrue(isSorted(LIST5, 5));

  reverse(LIST5, 5);
  assertFalse(isSorted(LIST5, 5));
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
#ifndef EPOXY_DUINO
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // for the Arduino Leonardo/Micro only
}

void loop() {
  aunit::TestRunner::run();
}
