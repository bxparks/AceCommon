#line 2 "LinearSearchTest.ino"

#include <Arduino.h>
#include <AUnit.h>
#include <AceCommon.h>

using aunit::TestRunner;
using ace_common::linearSearch;
using ace_common::linearSearchByKey;

//-----------------------------------------------------------------------------

static const int LIST[] = {
  -2,
  400,
  3,
  100,
  30,
};

static const size_t SIZE = sizeof(LIST)/sizeof(LIST[0]);

test(linearSearchTest, simple) {
  assertEqual(SIZE_MAX, linearSearch(LIST, SIZE, -10));
  assertEqual((size_t) 0, linearSearch(LIST, SIZE, -2));
  assertEqual((size_t) 2, linearSearch(LIST, SIZE, 3));
  assertEqual(SIZE_MAX, linearSearch(LIST, SIZE, 10));
  assertEqual((size_t) 4, linearSearch(LIST, SIZE, 30));
  assertEqual((size_t) 3, linearSearch(LIST, SIZE, 100));
  assertEqual(SIZE_MAX, linearSearch(LIST, SIZE, 9999));
}

// Check that the template method works for pointer to list.
test(linearSearchTest, simple_pointer_to_list) {
  const int* list = LIST;
  assertEqual((size_t) 2, linearSearch(list, SIZE, 3));
}

//-----------------------------------------------------------------------------

static const int LIST2[] = {
  0,
  2,
  2,
  1,
};

static const size_t SIZE2 = sizeof(LIST2)/sizeof(LIST2[0]);

test(linearSearchTest, simple_with_dups) {
  assertEqual((size_t) 0, linearSearch(LIST2, SIZE2, 0));
  size_t dupIndex = linearSearch(LIST2, SIZE2, 2);
  assertTrue(dupIndex == 1 || dupIndex == 2);
}

//-----------------------------------------------------------------------------

struct Record {
  int a;
  int b;
};

static const Record RECORDS[] = {
  {1, 2},
  {5, 6},
  {3, 4},
  {7, 8},
};

static const size_t NUM_RECORDS = sizeof(RECORDS) / sizeof(RECORDS[0]);

// Key mapping function that returns the value at index 'i'.
inline int key(size_t i) { return RECORDS[i].b; }

// Test linear search by key using a function.
test(linearSearchTest, records_with_function) {
  assertEqual(SIZE_MAX, linearSearchByKey(NUM_RECORDS, 0, key));
  assertEqual((size_t) 0, linearSearchByKey(NUM_RECORDS, 2, key));
  assertEqual((size_t) 2, linearSearchByKey(NUM_RECORDS, 4, key));
  assertEqual(SIZE_MAX, linearSearchByKey(NUM_RECORDS, 5, key));
  assertEqual((size_t) 1, linearSearchByKey(NUM_RECORDS, 6, key));
  assertEqual((size_t) 3, linearSearchByKey(NUM_RECORDS, 8, key));
}

// Test linear search by key using inlined lambda expression.
test(linearSearchTest, records_with_lambda) {
  assertEqual(
      SIZE_MAX,
      linearSearchByKey(NUM_RECORDS, 10,
          [](size_t i) { return RECORDS[i].b; } /*key*/)
  );
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
