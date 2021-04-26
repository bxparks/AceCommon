#include <Arduino.h>
#include <AceCommon.h>
#include "url_coding.hpp"

using namespace ace_common;

const int NUM_SAMPLES = 20;

#if defined(__linux__) || defined(__APPLE__)
  const unsigned FORM_URL_ENCODE_SIZES[] = {1000, 2000, 4000, 8000};
  const unsigned URL_ENCODE_NO_YIELD_SIZES[] = {1000, 2000, 4000, 8000};
  const unsigned URL_ENCODE_YIELD_SIZES[] = {5, 10, 20, 40};
#elif defined(ESP8266)
  const unsigned FORM_URL_ENCODE_SIZES[] = {1000, 2000, 4000, 8000};
  const unsigned URL_ENCODE_NO_YIELD_SIZES[] = {100, 200, 400, 800};
  const unsigned URL_ENCODE_YIELD_SIZES[] = {100, 200, 400, 800};
#elif defined(ESP32)
  #ifndef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR Serial
  #endif
  const unsigned FORM_URL_ENCODE_SIZES[] = {1000, 2000, 4000, 8000};
  const unsigned URL_ENCODE_NO_YIELD_SIZES[] = {100, 200, 400, 800};
  const unsigned URL_ENCODE_YIELD_SIZES[] = {100, 200, 400, 800};
#elif defined(ARDUINO_ARCH_AVR)
  const unsigned FORM_URL_ENCODE_SIZES[] = {25, 50, 100};
  const unsigned URL_ENCODE_NO_YIELD_SIZES[] = {25, 50, 100};
  const unsigned URL_ENCODE_YIELD_SIZES[] = {25, 50, 100};
#else
  const unsigned FORM_URL_ENCODE_SIZES[] = {100, 200, 400, 800};
  const unsigned URL_ENCODE_NO_YIELD_SIZES[] = {100, 200, 400, 800};
  const unsigned URL_ENCODE_YIELD_SIZES[] = {100, 200, 400, 800};
#endif

// A volatile variable that's updated at the end of the benchmark routine
// to prevent the compiler from optimizing away the entire calculation because
// it determines that there is no side-effect.
volatile uint8_t guard;

/** Create a random message of length size. */
void createMessage(Print& message, uint16_t size) {
  for (uint16_t i = 0; i < size - 1; i++) {
    long ran = random(127 - 32);
    char c = ran + 32;
    message.write(c);
  }
}

//----------------------------------------------------------------------------
// formUrlEncode()
//----------------------------------------------------------------------------

void benchmarkFormUrlEncode(TimingStats& stats, const char* message) {
  size_t messageLength = strlen(message);
  PrintStrN printString(2 * messageLength);
  yield();

  for (int i = 0; i < NUM_SAMPLES; i++) {
    uint16_t nowMicros = micros();
    formUrlEncode(printString, message);
    uint16_t elapsed = micros() - nowMicros;

    yield();
    guard = printString.length();
    stats.update(elapsed);
  }
}

void printFormUrlEncodeTime(uint16_t size) {
  yield();
  PrintStrN message(size);
  createMessage(message, size);
  TimingStats stats;
  benchmarkFormUrlEncode(stats, message.cstr());
  uint16_t perMil = (stats.getAvg() * 1000L) / size;

#if defined(ESP32) || defined(ESP8266)
  SERIAL_PORT_MONITOR.printf(
      "formUrlEncode(%4u)       | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#else
  printfTo(SERIAL_PORT_MONITOR,
      "formUrlEncode(%4u)       | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#endif
}

//----------------------------------------------------------------------------
// formUrlDecode()
//----------------------------------------------------------------------------

void benchmarkFormUrlDecode(TimingStats& stats, const char* encoded) {
  size_t encodedLength = strlen(encoded);
  PrintStrN printString(encodedLength);
  yield();

  for (int i = 0; i < NUM_SAMPLES; i++) {
    uint16_t nowMicros = micros();
    formUrlDecode(printString, encoded);
    uint16_t elapsed = micros() - nowMicros;

    yield();
    guard = printString.length();
    stats.update(elapsed);
  }
}

void printFormUrlDecodeTime(uint16_t size) {
  // Create a random message
  PrintStrN message(size);
  yield();
  createMessage(message, size);

  // encode it
  PrintStrN encoded(2 * size);
  yield();
  formUrlEncode(encoded, message.cstr());

  TimingStats stats;
  benchmarkFormUrlDecode(stats, encoded.cstr());
  uint16_t perMil = (stats.getAvg() * 1000L) / size;

#if defined(ESP32) || defined(ESP8266)
  SERIAL_PORT_MONITOR.printf(
      "formUrlDecode(%4u)       | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#else
  printfTo(SERIAL_PORT_MONITOR,
      "formUrlEncode(%4u)       | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#endif
}

//----------------------------------------------------------------------------
// urlencode_yield()
//----------------------------------------------------------------------------

void benchmarkUrlEncodeYield(TimingStats& stats, const char* message) {
  String msg(message);
  yield();

  for (int i = 0; i < NUM_SAMPLES; i++) {
    uint16_t nowMicros = micros();
    String encoded = urlencode_yield(msg);
    uint16_t elapsed = micros() - nowMicros;

    yield();
    guard = encoded.length();
    stats.update(elapsed);
  }
}

void printUrlEncodeYieldTime(uint16_t size) {
  yield();
  PrintStrN message(size);
  createMessage(message, size);
  TimingStats stats;
  benchmarkUrlEncodeYield(stats, message.cstr());
  uint16_t perMil = (stats.getAvg() * 1000L) / size;

#if defined(ESP8266) || defined(ESP32)
  SERIAL_PORT_MONITOR.printf(
      "urlencode_yield(%4u)     | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#else
  printfTo(SERIAL_PORT_MONITOR,
      "urlencode_yield(%4u)     | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#endif
}

//----------------------------------------------------------------------------
// urldecode_yield()
//----------------------------------------------------------------------------

void benchmarkUrlDecodeYield(TimingStats& stats, const char* encoded) {
  String encMsg(encoded);
  size_t encodedLength = strlen(encoded);
  PrintStrN printString(encodedLength);
  yield();

  for (int i = 0; i < NUM_SAMPLES; i++) {
    uint16_t nowMicros = micros();
    String decoded = urldecode_yield(encMsg);
    uint16_t elapsed = micros() - nowMicros;

    yield();
    guard = decoded.length();
    stats.update(elapsed);
  }
}

void printUrlDecodeYieldTime(uint16_t size) {
  // Create a random message
  PrintStrN message(size);
  yield();
  createMessage(message, size);

  // encode it
  PrintStrN encoded(2 * size);
  yield();
  formUrlEncode(encoded, message.cstr());

  TimingStats stats;
  benchmarkUrlDecodeYield(stats, encoded.cstr());
  uint16_t perMil = (stats.getAvg() * 1000L) / size;

#if defined(ESP32) || defined(ESP8266)
  SERIAL_PORT_MONITOR.printf(
      "urldecode_yield(%4u)     | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#else
  printfTo(SERIAL_PORT_MONITOR,
      "urldecode_yield(%4u)     | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#endif
}

//----------------------------------------------------------------------------
// urlencode_no_yield()
//----------------------------------------------------------------------------

void benchmarkUrlEncodeNoYield(TimingStats& stats, const char* message) {
  String msg(message);
  yield();

  for (int i = 0; i < NUM_SAMPLES; i++) {
    uint16_t nowMicros = micros();
    String encoded = urlencode_no_yield(msg);
    uint16_t elapsed = micros() - nowMicros;

    yield();
    guard = encoded.length();
    stats.update(elapsed);
  }
}

void printUrlEncodeNoYieldTime(uint16_t size) {
  yield();
  PrintStrN message(size);
  createMessage(message, size);
  TimingStats stats;
  benchmarkUrlEncodeNoYield(stats, message.cstr());
  uint16_t perMil = (stats.getAvg() * 1000L) / size;

#if defined(ESP32) || defined(ESP8266)
  SERIAL_PORT_MONITOR.printf(
      "urlencode_no_yield(%4u)  | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#else
  printfTo(SERIAL_PORT_MONITOR,
      "urlencode_no_yield(%4u)  | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#endif
}

//----------------------------------------------------------------------------
// urldecode_no_yield()
//----------------------------------------------------------------------------

void benchmarkUrlDecodeNoYield(TimingStats& stats, const char* encoded) {
  String encMsg(encoded);
  size_t encodedLength = strlen(encoded);
  PrintStrN printString(encodedLength);
  yield();

  for (int i = 0; i < NUM_SAMPLES; i++) {
    uint16_t nowMicros = micros();
    String decoded = urldecode_no_yield(encMsg);
    uint16_t elapsed = micros() - nowMicros;

    yield();
    guard = decoded.length();
    stats.update(elapsed);
  }
}

void printUrlDecodeNoYieldTime(uint16_t size) {
  // Create a random message
  PrintStrN message(size);
  yield();
  createMessage(message, size);

  // encode it
  PrintStrN encoded(2 * size);
  yield();
  formUrlEncode(encoded, message.cstr());

  TimingStats stats;
  benchmarkUrlDecodeNoYield(stats, encoded.cstr());
  uint16_t perMil = (stats.getAvg() * 1000L) / size;

#if defined(ESP32) || defined(ESP8266)
  SERIAL_PORT_MONITOR.printf(
      "urldecode_no_yield(%4u)  | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#else
  printfTo(SERIAL_PORT_MONITOR,
      "urldecode_no_yield(%4u)  | %6u | %6u | %6u | %6u |\n",
      size, stats.getAvg(), perMil, stats.getMin(), stats.getMax());
#endif
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000);
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // Wait until ready - Leonardo/Micro

  SERIAL_PORT_MONITOR.print(F("sizeof(PrintStrBase): "));
  SERIAL_PORT_MONITOR.println(sizeof(PrintStrBase));
  SERIAL_PORT_MONITOR.print(F("sizeof(PrintStrN): "));
  SERIAL_PORT_MONITOR.println(sizeof(PrintStrN));
  SERIAL_PORT_MONITOR.print(F("sizeof(PrintStr<8>): "));
  SERIAL_PORT_MONITOR.println(sizeof(PrintStr<8>));


  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  SERIAL_PORT_MONITOR.println(
      F("Description               | micros | us/1000|    min |    max |"));

  // formUrlEncode()
  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  for (unsigned i = 0; i < sizeof(FORM_URL_ENCODE_SIZES) / sizeof(unsigned);
      i++) {
    printFormUrlEncodeTime(FORM_URL_ENCODE_SIZES[i]);
  }

  // formUrlDecode()
  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  for (unsigned i = 0; i < sizeof(FORM_URL_ENCODE_SIZES) / sizeof(unsigned);
      i++) {
    printFormUrlDecodeTime(FORM_URL_ENCODE_SIZES[i]);
  }

  // urlencode_no_yield()
  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  for (unsigned i = 0; i < sizeof(URL_ENCODE_NO_YIELD_SIZES) / sizeof(unsigned);
      i++) {
    printUrlEncodeNoYieldTime(URL_ENCODE_NO_YIELD_SIZES[i]);
  }

  // urldecode_no_yield()
  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  for (unsigned i = 0; i < sizeof(URL_ENCODE_NO_YIELD_SIZES) / sizeof(unsigned);
      i++) {
    printUrlDecodeNoYieldTime(URL_ENCODE_NO_YIELD_SIZES[i]);
  }

  // urlencode_yield()
  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  for (unsigned i = 0; i < sizeof(URL_ENCODE_YIELD_SIZES) / sizeof(unsigned);
      i++) {
    printUrlEncodeYieldTime(URL_ENCODE_YIELD_SIZES[i]);
  }

  // urldecode_yield()
  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  for (unsigned i = 0; i < sizeof(URL_ENCODE_YIELD_SIZES) / sizeof(unsigned);
      i++) {
    printUrlDecodeYieldTime(URL_ENCODE_YIELD_SIZES[i]);
  }

  SERIAL_PORT_MONITOR.println(
      F("--------------------------+--------+--------+--------+--------+"));
  SERIAL_PORT_MONITOR.print(F("Num iterations: "));
  SERIAL_PORT_MONITOR.println(NUM_SAMPLES);

#if defined(EPOXY_DUINO)
  exit(0);
#endif
}

void loop() {
}
