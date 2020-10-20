/*
 * Compare the speed of ace_common::udiv1000() versus native
 * division by 1000 for various platforms.
 *
 * Summary:
 *
 *    * udiv1000() is faster than native division for AVR, SAMD21, and ESP8266.
 *    * udiv1000() is roughtly the same as native division for Teensy 3.2.
 *    * udiv1000 is *slower* than native divsion on ESP32, indicating that it
 *      has good hardware integer division support.
 *    * udiv1000() is *slower* than native division on 64-bit Linux on an
 *      Intel Core i7-3840QM processor.
 */

#include <Arduino.h>
#include <AceCommon.h>
using namespace ace_common;

#if defined(ARDUINO_ARCH_AVR)
  static const unsigned long COUNT = 100L*1000;
#elif defined(ARDUINO_ARCH_SAMD)
  static const unsigned long COUNT = 500L*1000;
#elif defined(ESP8266)
  static const unsigned long COUNT = 1L*1000*1000;
#elif defined(ESP32)
  static const unsigned long COUNT = 10L*1000*1000;
  #ifndef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR Serial
  #endif
#elif defined(TEENSYDUINO)
  static const unsigned long COUNT = 10L*1000*1000;
#elif defined(UNIX_HOST_DUINO)
  static const unsigned long COUNT = 1000L*1000*1000;
#else
  #error Unknown platform
#endif
static const unsigned long STEP = 1000L*1000*1000 / COUNT;

static volatile unsigned long guard; // prevent compiler optimization

void printResults(unsigned long approx, unsigned long native) {
  SERIAL_PORT_MONITOR.print(F("ApproxUdiv1000="));
  SERIAL_PORT_MONITOR.print(approx);
  SERIAL_PORT_MONITOR.print(F("ms; NativeUdiv1000="));
  SERIAL_PORT_MONITOR.print(native);
  SERIAL_PORT_MONITOR.println(F("ms"));
}

unsigned long runApproxUdiv1000(unsigned long n) {
  unsigned long startMillis = millis();
  unsigned long t = 0;
  for (unsigned long i = 0; i < n; i++, t+=STEP) {
    guard = udiv1000(i);
  }
  return millis() - startMillis;
}

unsigned long runNativeUdiv1000(unsigned long n) {
  unsigned long startMillis = millis();
  unsigned long t = 0;
  for (unsigned long i = 0; i < n; i++, t+=STEP) {
    guard = i / 1000;
  }
  return millis() - startMillis;
}

void setup() {
#if ! defined(UNIX_HOST_DUINO)
  delay(1000);
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // Leonardo/Micro

  unsigned long elapsedApprox = runApproxUdiv1000(COUNT);
  unsigned long elapsedNative = runNativeUdiv1000(COUNT);
  printResults(elapsedApprox, elapsedNative);
#if defined(UNIX_HOST_DUINO)
  exit(0);
#endif
}

void loop() {
}
