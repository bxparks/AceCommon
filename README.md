# AceCommon

[![AUnit Tests](https://github.com/bxparks/AceCommon/actions/workflows/aunit_tests.yml/badge.svg)](https://github.com/bxparks/AceCommon/actions/workflows/aunit_tests.yml)

An Arduino library containing small, low-level functions and routines which have
no dependencies to any other external libraries so that they can be easily
reused from other Arduino libraries in my collection.

All functions and classes are accessible through a single include file
`<AceCommon.h>` and the `ace_common` namespace:

```C++
#include <AceCommon.h>
using namespace ace_common;
```

The `src/AceCommon.h` header file includes the following sub-header files
automatically:

* [src/arithmetic/arithmetic.h](src/arithmetic/arithmetic.h)
    * `void incrementMod(T& c, T m)`
    * `void incrementModOffset(T&c, T m, T offset)`
    * `uint8_t decToBcd(uint8_t)`
    * `uint8_t bcdToDec(uint8_t)`
    * `unsigned long udiv1000(unsigned long)`
* [src/pstrings/pstrings.h](src/pstrings/pstrings.h)
    * `int strcmp_PP(const char* a, const char* b)`
    * `const char* strchr_P(const char* s, char c)` (ESP8266 and ESP32 only)
    * `const char* strrchr_P(const char* s, char c)` (ESP8266 and ESP32 only)
* [src/print_str/PrintStr.h](src/print_str/PrintStr.h)
    * [src/print_str/README.md](src/print_str/README.md)
        * Provides classes that implement the `Print` interface so that
          values can be printed into in-memory buffers. The string can then
          be extracted as a normal c-string using `const char*
          PrintStr::cstr()`.
        * Alternative to the Arduino `String` class to avoid or reduce heap
          fragmentation.
    * `class PrintStrBase`
    * `class PrintStr<uint16_t SIZE>` (buffer on stack)
    * `class PrintStrN(uint16_t size)` (buffer on heap)
* [src/print_utils/printPadTo.h](src/print_utils/printPadTo.h)
    * [src/print_utils/README.md](src/print_utils/README.md)
    * `void printPad2To(Print& printer, uint16_t val, char pad = ' ')`
    * `void printPad3To(Print& printer, uint16_t val, char pad = ' ')`
    * `void printPad4To(Print& printer, uint16_t val, char pad = ' ')`
    * `void printPad5To(Print& printer, uint16_t val, char pad = ' ')`
* [src/print_utils/printIntAsFloat.h](src/print_utils/printIntAsFloat.h)
    * `void printUint16AsFloat3To(Print& printer, uint16_t val)`
        * Divide 16-bit integer `val` by 1000 and print the result as a floating
          point number to 3 decimal places.
        * Does not use floating point operations.
    * `void printUint32AsFloat3To(Print& printer, uint32_t val)`
        * Divide 32-bit integer `val` by 1000 and print the result as a floating
          point number to 3 decimal places.
        * Does not use floating point operations.
* [src/print_utils/printfTo.h](src/print_utils/printfTo.h)
    * [src/print_utils/README.md](src/print_utils/README.md)
        * Provides a primitive `printf()` functionality to an instance of
          `Print` (e.g. `Serial`) for those Arduino boards without a
          `Print.printf()` function.
    * `void printfTo(Print& printer, const char* fmt, ...)`
* [src/timing_stats/TimingStats.h](src/timing_stats/TimingStats.h)
    * [src/timing_stats/README.md](src/timing_stats/README.md)
        * Helper class to collect data (often durations in milliseconds) and
          then print out various statistics such as min, max, average, and
          count.
    * `class TimingStats`
* [src/timing_stats/GenericStats.h](src/timing_stats/GenericStats.h)
    * Same as `TimingStats` but templatized to support generic type `T`
    * `class GenericStats<T>`
* [src/url_encoding/url_encoding.h](src/url_encoding/url_encoding.h)
    * [src/url_encoding/README.md](src/url_encoding/README.md)
        * Encodes and decodes strings using "form URL encoding" which converts
          spaces `' '` into `'+'`, and non-alphnumerics into percent-hex digits.
    * `void formUrlEncode(Print& output, const char* str)`
    * `void formUrlDecode(Print& output, const char* str)`
* [src/fstrings/FCString.h](src/fstrings/FCString.h)
    * `class FCString`
    * An object that can hold either a C-string (`const char*`) or an
      F-string (`const __FlashStringHelper*`).
* [src/hash/djb2.h](src/hash/djb2.h)
    * The djb2 hash algorithm as described in
      https://stackoverflow.com/questions/7666509 and
      http://www.cse.yorku.ca/~oz/hash.html.
    * `uint32_t hashDjb2(const char* s)`
    * `uint32_t hashDjb2(const __FlashStringHelper* fs)`
* [src/kstrings/KString.h](src/kstrings/KString.h)
    * Wrapper around a c-string or an f-string which supports compression
      using keyword substitution from a dictionary. Up to 31 keywords supported.
    * `class KString`
* [src/algorithms/binarySearch.h](src/algorithms/binarySearch.h)
    * [src/algorithms/README.md](src/algorithms/README.md)
    * `size_t binarySearchByKey(size_t size, const X& x, K&& key)`
    * `size_t binarySearch(const X list[], size_t size, const X& x)`
        * Templatized binary search of array of records or array of elements.
* [src/algorithms/linearSearch.h](src/algorithms/linearSearch.h)
    * [src/algorithms/README.md](src/algorithms/README.md)
    * `size_t linearSearchByKey(size_t size, const X& x, K&& key)`
    * `size_t linearSearch(const X list[], size_t size, const X& x)`
        * Templatized linear search of array of records or array of elements.
* [src/algorithms/isSorted.h](src/algorithms/isSorted.h)
    * [src/algorithms/README.md](src/algorithms/README.md)
    * Determine if array is sorted or not.
    * `size_t isSorted(const X list[], size_t size)`
    * `size_t isSortedByKey(size_t size, K&& key)`
    * `size_t isReverseSorted(const X list[], size_t size)`
    * `size_t isReverseSortedByKey(size_t size, K&& key)`
* [src/algorithms/reverse.h](src/algorithms/reverse.h)
    * [src/algorithms/README.md](src/algorithms/README.md)
    * `void reverse(T data[], size_t size)`
* [src/cstrings/copyReplace.h](src/cstrings/copyReplace.h)
    * Replace a character with another character or string and copy result to
      destination.
    * `void copyReplaceChar(char* dst, size_t dstSize, const char* src,
      char oldChar, char newChar)`
    * `void copyReplaceString(char* dst, size_t dstSize, const char* src,
      char oldChar, const char* newString)`
* [src/print_utils/printReplaceTo.h](src/print_utils/printReplaceTo.h)
    * Print a string while replacing a character with another character or
      another string.
    * `void printReplaceCharTo(
      Print& printer, const char* src, char oldChar, char newChar)`
    * `void printReplaceCharTo(
      Print& printer, const __FlashStringHelper* src, char oldChar,
      char newChar)`
    * `void printReplaceStringTo(
      Print& printer, const char* src, char oldChar, const char* newString)`
    * `void printReplaceStringTo(
      Print& printer, const __FlashStringHelper* src, char oldChar,
      const char* newString)`
* [src/fstrings/FlashString.h](src/fstrings/FlashString.h)
    * [src/fstrings/README.md](src/fstrings/README.md)
    * `class FlashString`
    * Wraps a `const __FlashStringHelper*` into an object that looks and acts
      like a normal c-string pointer `const char*`.
    * Allows template functions to be written once, then reused for a flash
      string pointer `const __FlashStringHelper*`.

**Version**: 1.4.7 (2021-11-04)

**Changelog**: [CHANGELOG.md](CHANGELOG.md)

## Installation

The latest stable release is available in the Arduino IDE Library Manager.
Search for "AceCommon". Click install.

The development version can be installed by cloning the
GitHub repository (https://github.com/bxparks/AceCommon), checking out the
`develop` branch, then manually copying over the contents to the `./libraries`
directory used by the Arduino IDE. (The result is a directory named
`./libraries/AceCommon`.)

The `master` branch contains the tagged stable releases.

### External Dependencies

The core of this library will never depend on any external dependencies so that
it can be easily reused by other libraries (e.g. AceTime, AceButton, AceRoutine,
etc).

The unit tests under `tests/` depend on:

* AUnit (https://github.com/bxparks/AUnit)

The unit tests can be executed on Linux or MacOS using:

* EpoxyDuino (https://github.com/bxparks/EpoxyDuino)

### Source Code

The source files are organized as follows:

* `src/AceCommon.h` - main header file
* `src/*/` - implementation files
* `tests/` - unit tests which require [AUnit](https://github.com/bxparks/AUnit)
* `examples/` - example sketches
* `examples/MemoryBenchmark`
    * gathers the flash and static memory consumption of various functions
      and classes
    * validates my intuition of memory consumption of these routines
    * detects unexpected changes in memory consumption
* `examples/AutoBenchmark`
    * determines the CPU time consumed by various AceCommon features or
      functions

### Documentation

* [README.md](README.md) - this file
* [Doxygen docs](https://bxparks.github.io/AceCommon/html/) published on GitHub
  Pages

<a name="SystemRequirements"></a>
## System Requirements

### Hardware

The library has been extensively tested on the following boards:

* Arduino Nano clone (16 MHz ATmega328P)
* SparkFun Pro Micro clone (16 MHz ATmega32U4)
* SAMD21 M0 Mini (48 MHz ARM Cortex-M0+)
* STM32 Blue Pill (STM32F103C8, 72 MHz ARM Cortex-M3)
* NodeMCU 1.0 (ESP-12E module, 80MHz ESP8266)
* WeMos D1 Mini (ESP-12E module, 80 MHz ESP8266)
* ESP32 Dev Module (ESP-WROOM-32 module, 240MHz dual core Tensilica LX6)
* Teensy 3.2 (96 MHz ARM Cortex-M4)

I will occasionally test on the following boards as a sanity check:

* Arduino Pro Mini clone (16 MHz ATmega328P)
* Mini Mega 2560 (Arduino Mega 2560 compatible, 16 MHz ATmega2560)
* Teensy LC (48 MHz ARM Cortex-M0+)

The following boards are *not* supported:

* megaAVR (e.g. Nano Every)
* SAMD21 boards w/ `arduino:samd` version >= 1.8.10 (e.g. MKRZero)

### Tool Chain

This library was developed and tested using:

* [Arduino IDE 1.8.16](https://www.arduino.cc/en/Main/Software)
* [Arduino CLI 0.19.2](https://arduino.github.io/arduino-cli)
* [Arduino AVR Boards 1.8.3](https://github.com/arduino/ArduinoCore-avr)
* [Arduino SAMD Boards 1.8.9](https://github.com/arduino/ArduinoCore-samd)
* [SparkFun AVR Boards 1.1.13](https://github.com/sparkfun/Arduino_Boards)
* [SparkFun SAMD Boards 1.8.5](https://github.com/sparkfun/Arduino_Boards)
* [STM32duino 2.0.0](https://github.com/stm32duino/Arduino_Core_STM32)
* [ESP8266 Arduino Core 3.0.2](https://github.com/esp8266/Arduino)
* [ESP32 Arduino Core 1.0.6](https://github.com/espressif/arduino-esp32)
* [Teensyduino 1.55](https://www.pjrc.com/teensy/td_download.html)

It should work with [PlatformIO](https://platformio.org/) but I have
not tested it.

The library works on Linux or MacOS (using both g++ and clang++ compilers) using
the [EpoxyDuino](https://github.com/bxparks/EpoxyDuino) emulation layer.

### Operating System

I use Ubuntu Linux 18.04 and 20.04 for most of my development.

## License

[MIT License](https://opensource.org/licenses/MIT)

## Feedback and Support

If you have any questions, comments, or feature requests for this library,
please use the [GitHub
Discussions](https://github.com/bxparks/AceCommon/discussions) for this project.
If you have bug reports, please file a ticket in [GitHub
Issues](https://github.com/bxparks/AceCommon/issues). Feature requests should go
into Discussions first because they often have alternative solutions which are
useful to remain visible, instead of disappearing from the default view of the
Issue tracker after the ticket is closed.

Please refrain from emailing me directly unless the content is sensitive. The
problem with email is that I cannot reference the email conversation when other
people ask similar questions later.

## Author

Created by Brian T. Park (brian@xparks.net).
