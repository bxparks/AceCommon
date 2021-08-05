/*
 * MIT License
 * Copyright (c) 2018 Brian T. Park
 */

#include <Arduino.h> // F(), __FlashStringHelper
#include <AceCommon.h> // printPad3To(), TimingStats
#include "Benchmark.h"

#ifndef SERIAL_PORT_MONITOR
  #define SERIAL_PORT_MONITOR Serial
#endif

using namespace ace_common;

//-----------------------------------------------------------------------------
// Benchmark configs
//-----------------------------------------------------------------------------

static const uint8_t SAMPLE_SIZE = 5;

#if defined(ARDUINO_ARCH_AVR)
  static const uint32_t LOOP_COUNT = 1UL*1000;
#elif defined(ARDUINO_ARCH_SAMD)
  static const uint32_t LOOP_COUNT = 5UL*1000;
#elif defined(ARDUINO_ARCH_STM32)
  static const uint32_t LOOP_COUNT = 10UL*1000;
#elif defined(ESP8266)
  static const uint32_t LOOP_COUNT = 10UL*1000;
#elif defined(ESP32)
  static const uint32_t LOOP_COUNT = 1UL*1000*1000;
#elif defined(TEENSYDUINO)
  static const uint32_t LOOP_COUNT = 1UL*1000*1000;
#elif defined(EPOXY_DUINO)
  static const uint32_t LOOP_COUNT = 1UL*1000*1000;
#else
  #error Unknown platform
#endif

//-----------------------------------------------------------------------------
// Benchmark helpers
//-----------------------------------------------------------------------------

/**
 * Print micros per count as a floating point number with 3 decimal places.
 * For example, if `us=13032` and `count=2000`, this prints "6.516".
 */
void printMicros(uint32_t us, uint32_t count) {
  uint32_t nanos = us * 1000 / count;
  printUint32AsFloat3To(SERIAL_PORT_MONITOR, nanos);
}

/**
 * Print the result in micros for the given 'name' function or algorithm. The
 * format is:
 *
 * @verbatim
 * name min avg max loopCount sampleSize
 * @endverbatim
 *
 * @param name name of the benchmark
 * @param instance of TimingStats
 * @param loopCount number of inner loops which were used to build the
 *        timingStats
 * @param samplesize number of independent timingStats samples used, to get
 *        the min(), avg() and max()
 */
static void printStats(
    const __FlashStringHelper* name,
    const TimingStats& stats,
    uint32_t loopCount,
    uint32_t sampleSize) {
  SERIAL_PORT_MONITOR.print(name);
  SERIAL_PORT_MONITOR.print(' ');
  printMicros(stats.getMin(), loopCount);
  SERIAL_PORT_MONITOR.print(' ');
  printMicros(stats.getAvg(), loopCount);
  SERIAL_PORT_MONITOR.print(' ');
  printMicros(stats.getMax(), loopCount);
  SERIAL_PORT_MONITOR.print(' ');
  SERIAL_PORT_MONITOR.print(loopCount);
  SERIAL_PORT_MONITOR.print(' ');
  SERIAL_PORT_MONITOR.println(sampleSize);
}

static TimingStats timingStats;

// Volatile to prevent the compiler from deciding that the entire program does
// nothing, causing it to opimize the whole program to nothing.
volatile uint32_t disableCompilerOptimization;

// Typedef of the lambda expression that can be converted into a simple function
// pointer.
typedef void (*Lambda)();

/**
 * Return the number of micros ellapsed during the execution of the lambda. The
 * lambda function should contain a loop that runs the thing under test enough
 * times for this to return an ellapsed time in the range of roughly 1-20
 * micros. Shorter than that, we risk increasing the measurement error. Longer
 * than that, the ESP2866 may reboot due to the watchdog timer.
 *
 * The lambda should also write something to the `disableCompilerOptimization`
 * so that the compiler does optimize way the entire thing.
 *
 * In most cases, we should also run an empty-loop that does nothing but loop
 * for the same number of times, then update the `disableCompilerOptimization`,
 * and subtract its result from the previous result, to remove the overhead of
 * the loop construct itself. Sometime, when this is done, the number is
 * negative if the thing being tested run so quickly (compared to a single
 * iteration of the empty loop), that the measurement error is bigger than the
 * benchmark timing. The printStats() function needs to be able to handle that.
 */
static uint32_t runLambda(Lambda lambda) {
  yield();
  uint32_t startMicros = micros();
  lambda();
  uint32_t elapsedMicros = micros() - startMicros;
  yield();
  return elapsedMicros;
}

//-----------------------------------------------------------------------------
// Benchmarks
//-----------------------------------------------------------------------------

static void runUdiv1000Native() {
  timingStats.reset();

  for (uint8_t k = 0; k < SAMPLE_SIZE; k++) {

    // We must count by 1100 because the compiler is too smart and optimizes
    // away the loop if it knows that LOOP_COUNT is < 1000.
    uint32_t rawMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT*1100; i+=1100) {
        disableCompilerOptimization = (i+1) / 1000;
      }
    });

    uint32_t emptyMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT*1100; i+=1100) {
        disableCompilerOptimization = (i+1);
      }
    });

    uint32_t benchmarkMicros = rawMicros - emptyMicros;
    timingStats.update(benchmarkMicros);
  }

  printStats(F("/1000"), timingStats, LOOP_COUNT, SAMPLE_SIZE);
}

static void runUdiv1000() {
  timingStats.reset();

  for (uint8_t k = 0; k < SAMPLE_SIZE; k++) {

    // We must count by 1100 because the compiler is too smart and optimizes
    // away the loop if it knows that LOOP_COUNT is < 1000.
    uint32_t rawMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT * 1100; i+=1100) {
        disableCompilerOptimization = udiv1000(i+1);
      }
    });

    uint32_t emptyMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT*1100; i+=1100) {
        disableCompilerOptimization = i+1;
      }
    });

    uint32_t benchmarkMicros = rawMicros - emptyMicros;
    timingStats.update(benchmarkMicros);
  }

  printStats(F("udiv1000()"), timingStats, LOOP_COUNT, SAMPLE_SIZE);
}

static void runDecToBcdDivOnly() {
  timingStats.reset();

  for (uint8_t k = 0; k < SAMPLE_SIZE; k++) {

    // We must count by 1100 because the compiler is too smart and optimizes
    // away the loop if it knows that LOOP_COUNT is < 1000.
    uint32_t rawMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = decToBcdDivOnly(i & 0xFF);
      }
    });

    uint32_t emptyMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = i & 0xFF;
      }
    });

    uint32_t benchmarkMicros = rawMicros - emptyMicros;
    timingStats.update(benchmarkMicros);
  }

  printStats(F("decToBcdDivOnly()"), timingStats, LOOP_COUNT, SAMPLE_SIZE);
}

static void runDecToBcdDivMod() {
  timingStats.reset();

  for (uint8_t k = 0; k < SAMPLE_SIZE; k++) {

    // We must count by 1100 because the compiler is too smart and optimizes
    // away the loop if it knows that LOOP_COUNT is < 1000.
    uint32_t rawMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = decToBcdDivMod(i & 0xFF);
      }
    });

    uint32_t emptyMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = i & 0xFF;
      }
    });

    uint32_t benchmarkMicros = rawMicros - emptyMicros;
    timingStats.update(benchmarkMicros);
  }

  printStats(F("decToBcdDivMod()"), timingStats, LOOP_COUNT, SAMPLE_SIZE);
}

static void runDecToBcd() {
  timingStats.reset();

  for (uint8_t k = 0; k < SAMPLE_SIZE; k++) {

    // We must count by 1100 because the compiler is too smart and optimizes
    // away the loop if it knows that LOOP_COUNT is < 1000.
    uint32_t rawMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = decToBcd(i & 0xFF);
      }
    });

    uint32_t emptyMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = i & 0xFF;
      }
    });

    uint32_t benchmarkMicros = rawMicros - emptyMicros;
    timingStats.update(benchmarkMicros);
  }

  printStats(F("decToBcd()"), timingStats, LOOP_COUNT, SAMPLE_SIZE);
}

static void runBcdToDec() {
  timingStats.reset();

  for (uint8_t k = 0; k < SAMPLE_SIZE; k++) {

    // We must count by 1100 because the compiler is too smart and optimizes
    // away the loop if it knows that LOOP_COUNT is < 1000.
    uint32_t rawMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = bcdToDec(i & 0xFF);
      }
    });

    uint32_t emptyMicros = runLambda([]() {
      for (uint32_t i = 0; i < LOOP_COUNT; i++) {
        disableCompilerOptimization = i & 0xFF;
      }
    });

    uint32_t benchmarkMicros = rawMicros - emptyMicros;
    timingStats.update(benchmarkMicros);
  }

  printStats(F("bcdToDec()"), timingStats, LOOP_COUNT, SAMPLE_SIZE);
}

void runBenchmarks() {
  runUdiv1000Native();
  runUdiv1000();
  runDecToBcdDivOnly();
  runDecToBcdDivMod();
  runDecToBcd();
  runBcdToDec();
}
