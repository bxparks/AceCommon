#line 2 "PrintStrTest.ino"

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

//----------------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------------

static const int BUF_SIZE = 300;
static char STRING[BUF_SIZE];

static void fillString() {
  for (size_t i = 0; i < BUF_SIZE; i++) {
    char c = (i % 10) + '0';
    STRING[i] = c;
  }
  STRING[BUF_SIZE - 1] = '\0';
}

static void truncateString(int length) {
  STRING[length] = '\0';
}

//----------------------------------------------------------------------------
// Test that PrintStr<SiZE> can handle strings >= 255 but <= 65534.
// In practice, the SIZE is limited by the maximum stack size, which can be
// far smaller than the amount of static RAM avaiable. For example. ESP8266 has
// a max stack size of 4kB, even though it has 80kB of RAM.
//----------------------------------------------------------------------------

test(PrintStrTest, flush) {
  PrintStr<10> printStr;
  size_t n = printStr.print('a');
  assertEqual((size_t) 1, n);
  assertEqual((size_t) 1, printStr.length());

  printStr.flush();
  assertEqual("", printStr.cstr());
}

test(PrintStrTest, print_underSized) {
  fillString();
  PrintStr<310> printStr;
  size_t n = printStr.print(STRING);

  // Verify that the entire STRING got copied
  assertEqual((size_t) BUF_SIZE - 1, n);
  assertEqual((size_t) BUF_SIZE - 1, printStr.length());
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrTest, print_overSized) {
  fillString();
  PrintStr<270> printStr;
  size_t n = printStr.print(STRING);

  // Verify that the STRING was truncated to 269
  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrTest, write_overSized) {
  fillString();
  PrintStr<270> printStr;
  size_t n = printStr.write((const uint8_t*) STRING, BUF_SIZE - 1);

  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
}

//----------------------------------------------------------------------------
// Test PrintStrN(size) which creates the char array buffer on the heap.
//----------------------------------------------------------------------------

test(PrintStrNTest, flush) {
  PrintStrN printStr(10);
  size_t n = printStr.print('a');
  assertEqual((size_t) 1, n);
  assertEqual((size_t) 1, printStr.length());

  printStr.flush();
  assertEqual("", printStr.cstr());
}

test(PrintStrNTest, print_underSized) {
  fillString();
  PrintStrN printStr(310);
  size_t n = printStr.print(STRING);

  // Verify that the entire STRING got copied
  assertEqual((size_t) BUF_SIZE - 1, n);
  assertEqual((size_t) BUF_SIZE - 1, printStr.length());
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrNTest, print_overSized) {
  fillString();
  PrintStrN printStr(270);
  size_t n = printStr.print(STRING);

  // Verify that the STRING was truncated to 269
  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrNTest, write_overSized) {
  fillString();
  PrintStrN printStr(270);
  size_t n = printStr.write((const uint8_t*) STRING, BUF_SIZE - 1);

  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
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
