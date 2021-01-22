/*
 * Compare the speed of ace_common::udiv1000() versus native
 * division by 1000 for various platforms.
 *
 * Results:
 *
 * * Arduino Nano
 *    * ApproxUdiv1000=5.780us/iter; NativeUdiv1000=38.380us/iter
 * * SparkFun Pro Micro
 *    * ApproxUdiv1000=5.820us/iter; NativeUdiv1000=38.570us/iter
 * * SAMD21
 *    * ApproxUdiv1000=0.376us/iter; NativeUdiv1000=3.454us/iter
 * * STM32 "Blue Pill"
 *    * ApproxUdiv1000=0.223us/iter; NativeUdiv1000=0.235us/iter
 * * ESP8266
 *    * ApproxUdiv1000=0.163us/iter; NativeUdiv1000=1.476us/iter
 * * ESP32
 *    * ApproxUdiv1000=0.067us/iter; NativeUdiv1000=0.046us/iter
 * * Teensy 3.2
 *    * ApproxUdiv1000=0.125us/iter; NativeUdiv1000=0.093us/iter
 * * EpoxyDuino on Intel Core i7-3840QM, Ubuntu 20.04
 *    * ApproxUdiv1000=0.005us/iter; NativeUdiv1000=0.001us/iter
 *
 * * udiv1000() is faster than native division for AVR, SAMD21, and ESP8266.
 * * udiv1000() is roughtly the same as native division for Teensy 3.2.
 */

#include <Arduino.h>
#include <AceCommon.h> // printPad3To(), udiv1000()
using namespace ace_common;

#if defined(ARDUINO_ARCH_AVR)
  static const unsigned long COUNT = 100L*1000;
#elif defined(ARDUINO_ARCH_SAMD)
  static const unsigned long COUNT = 500L*1000;
#elif defined(ARDUINO_ARCH_STM32)
  static const unsigned long COUNT = 1L*1000*1000;
#elif defined(ESP8266)
  static const unsigned long COUNT = 1L*1000*1000;
#elif defined(ESP32)
  static const unsigned long COUNT = 10L*1000*1000;
  #ifndef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR Serial
  #endif
#elif defined(TEENSYDUINO)
  static const unsigned long COUNT = 10L*1000*1000;
#elif defined(EPOXY_DUINO)
  static const unsigned long COUNT = 100L*1000*1000;
#else
  #error Unknown platform
#endif

static volatile unsigned long guard; // prevent compiler optimization

void printMicros(unsigned long ms, unsigned long count) {
  unsigned long nanos = ms * 1000 / (count / 1000);
  uint16_t micros = nanos / 1000;
  uint16_t frac = nanos % 1000;
  SERIAL_PORT_MONITOR.print(micros);
  SERIAL_PORT_MONITOR.print('.');
  printPad3To(SERIAL_PORT_MONITOR, frac, '0');
}

void printResults(unsigned long approx, unsigned long native,
    unsigned long count) {
  SERIAL_PORT_MONITOR.print(F("ApproxUdiv1000="));
  printMicros(approx, count);
  SERIAL_PORT_MONITOR.print(F("us/iter; NativeUdiv1000="));
  printMicros(native, count);
  SERIAL_PORT_MONITOR.println(F("us/iter"));
}

unsigned long runApproxUdiv1000(unsigned long n) {
  unsigned long startMillis = millis();
  for (unsigned long i = 0; i < n; i++) {
    guard = udiv1000(i);
  }
  return millis() - startMillis;
}

unsigned long runNativeUdiv1000(unsigned long n) {
  unsigned long startMillis = millis();
  for (unsigned long i = 0; i < n; i++) {
    guard = i / 1000;
  }
  return millis() - startMillis;
}

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000);
#endif

  SERIAL_PORT_MONITOR.begin(115200);
  while (!SERIAL_PORT_MONITOR); // Leonardo/Micro

  unsigned long elapsedApprox = runApproxUdiv1000(COUNT);
  yield();
  unsigned long elapsedNative = runNativeUdiv1000(COUNT);
  yield();
  printResults(elapsedApprox, elapsedNative, COUNT);

#if defined(EPOXY_DUINO)
  exit(0);
#endif
}

void loop() {
}
