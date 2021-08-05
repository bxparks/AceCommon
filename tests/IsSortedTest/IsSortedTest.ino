#line 2 "IsSortedTest.ino"

#include <Arduino.h>
#include <AUnit.h>
#include <AceCommon.h>

using aunit::TestRunner;
using ace_common::isSorted;
using ace_common::isSortedByKey;
using ace_common::isReverseSorted;

//-----------------------------------------------------------------------------

static const int SORTED_LIST[] = {
  -2,
  3,
  30,
  100,
  400,
};

static const size_t SORTED_LIST_SIZE =
    sizeof(SORTED_LIST) / sizeof(SORTED_LIST[0]);

test(isSortedTest, empty_list) {
  assertTrue(isSorted(SORTED_LIST, 0));
}

test(isSortedTest, single_list) {
  assertTrue(isSorted(SORTED_LIST, 1));
}

test(isSortedTest, sorted_list) {
  assertTrue(isSorted(SORTED_LIST, SORTED_LIST_SIZE));
}

//-----------------------------------------------------------------------------

static const int REVERSE_SORTED_LIST[] = {
  400,
  100,
  30,
  3,
  -2,
};

static const size_t REVERSE_SORTED_LIST_SIZE =
    sizeof(REVERSE_SORTED_LIST) / sizeof(REVERSE_SORTED_LIST[0]);

test(isReversedSortedTest, empty_reversed) {
  assertTrue(isReverseSorted(REVERSE_SORTED_LIST, 0));
}

test(isReversedSortedTest, single_reversed) {
  assertTrue(isReverseSorted(REVERSE_SORTED_LIST, 1));
}

test(isReversedSortedTest, reversed_list) {
  assertTrue(isReverseSorted(REVERSE_SORTED_LIST, REVERSE_SORTED_LIST_SIZE));
}

//-----------------------------------------------------------------------------

static const int UNSORTED_LIST[] = {
  3,
  -2,
  30,
  100,
};

static const size_t UNSORTED_LIST_SIZE =
    sizeof(UNSORTED_LIST) / sizeof(UNSORTED_LIST[0]);

test(isSortedTest, unsorted_list) {
  assertFalse(isSorted(UNSORTED_LIST, UNSORTED_LIST_SIZE));
}

test(isReverseSortedTest, unsorted_list) {
  assertFalse(isReverseSorted(UNSORTED_LIST, UNSORTED_LIST_SIZE));
}

//-----------------------------------------------------------------------------

struct Record {
  int a;
  int b;
};

static const Record SORTED_RECORDS[] = {
  {1, 2},
  {3, 4},
  {5, 6},
  {7, 8},
};

static const size_t NUM_SORTED_RECORDS =
    sizeof(SORTED_RECORDS) / sizeof(SORTED_RECORDS[0]);

// Key mapping function that returns the value at index 'i'.
inline int key(size_t i) { return SORTED_RECORDS[i].b; }

// Test isSortedByKey() using a function.
test(isSortedByKeyTest, sorted_records_with_function) {
  assertTrue(isSortedByKey(NUM_SORTED_RECORDS, key));
}

// Test isSortedByKey() using a function, with N=0.
test(isSortedByKeyTest, zero_element) {
  assertTrue(isSortedByKey(0, key));
}

// Test isSortedByKey() using a function, with N=1.
test(isSortedByKeyTest, one_element) {
  assertTrue(isSortedByKey(1, key));
}

// Test isSortedByKey() using inlined lambda expression.
test(isSortedByKeyTest, sorted_records_with_lambda) {
  assertTrue(isSortedByKey(NUM_SORTED_RECORDS,
      [](size_t i) { return SORTED_RECORDS[i].b; } /*key*/)
  );
}

//-----------------------------------------------------------------------------

static const Record UNSORTED_RECORDS[] = {
  {1, 2},
  {5, 6},
  {3, 4},
  {7, 8},
};

static const size_t NUM_UNSORTED_RECORDS =
    sizeof(UNSORTED_RECORDS) / sizeof(UNSORTED_RECORDS[0]);

// Key mapping function that returns the value at index 'i'.
inline int keyForUnsorted(size_t i) { return UNSORTED_RECORDS[i].b; }

// Test isSortedByKey() using a function.
test(isSortedTest, unsorted_records_with_function) {
  assertFalse(isSortedByKey(NUM_UNSORTED_RECORDS, keyForUnsorted));
}

// Test isSortedByKey() using inlined lambda expression.
test(isSortedTest, unsorted_records_with_lambda) {
  assertFalse(isSortedByKey(NUM_UNSORTED_RECORDS,
      [](size_t i) { return UNSORTED_RECORDS[i].b; } /*key*/)
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
