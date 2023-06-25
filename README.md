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

Instead of pulling in all functions and classes in the `ace_common` namespace
into the global namespace, it is often better to limit the import to the
specific function that is needed. For example if only the `hashDjb2()` is
needed, then do this:

```C++
#include <AceCommon.h>
using ace_common::hashDjb2;
```

The `src/AceCommon.h` header file includes the following sub-header files
automatically:

**Arithmetic**

* [src/arithmetic/arithmetic.h](src/arithmetic/arithmetic.h)
    * `void incrementMod(T& c, T m)`
    * `void incrementModOffset(T&c, T m, T offset)`
    * `uint8_t decToBcd(uint8_t)`
    * `uint8_t bcdToDec(uint8_t)`
    * `unsigned long udiv1000(unsigned long)`

**String Types**

* [src/pstrings/pstrings.h](src/pstrings/pstrings.h)
    * `int strcmp_PP(const char* a, const char* b)`
    * `const char* strchr_P(const char* s, char c)` (ESP8266 and ESP32 only)
    * `const char* strrchr_P(const char* s, char c)` (ESP8266 and ESP32 only)
* [src/tstrings/tstrings.h](src/tstrings/tstrings.h)
    * Overloaded functions which provide a thin layer of indirection around a
      `const char*` or a `const __FlashStringHelper*`.
    * Allows writing of template code which is agnostic to whether the string
      is stored in normal memory or flash memory. The compiler will choose the
      exact implementation at compile-time.
    * `strcat_T()`
    * `strchr_T()`
    * `strcmp_T()`
    * `strcpy_T()`
    * `strlen_T()`
    * `strncat_T()`
    * `strncmp_T()`
    * `strncpy_T()`
    * `strrchr_T()`
* [src/fstrings/FCString.h](src/fstrings/FCString.h)
    * `class FCString`
    * An object that can hold either a C-string (`const char*`) or an
      F-string (`const __FlashStringHelper*`), and a type discrimination tag
      that allows the client code to determine the type at runtime.
* [src/fstrings/FlashString.h](src/fstrings/FlashString.h)
    * [src/fstrings/README.md](src/fstrings/README.md)
    * `class FlashString`
    * Wraps a `const __FlashStringHelper*` into an object that looks and acts
      like a normal c-string pointer `const char*`.
    * Allows template functions to be written once, then reused for a flash
      string pointer `const __FlashStringHelper*`.
* [src/kstrings/KString.h](src/kstrings/KString.h)
    * Wrapper around a c-string or an f-string which supports compression
      using keyword substitution from a dictionary. Up to 31 keywords supported.
    * The keywords dictionary itself can be in normal memory or flash memory.
    * `class KString`
* [src/cstrings/copyReplace.h](src/cstrings/copyReplace.h)
    * Replace a character with another character and copy result to destination.
      There are 2 overloaded versions:
        * `void copyReplaceChar(char* dst, size_t dstSize, const char* src,
        char oldChar, char newChar)`
        * `void copyReplaceChar(char* dst, size_t dstSize,
            const __FlashStringHelper* src, char oldChar, char newChar)`
    * Replace a character with another string and copy result to destination.
      There are 4 overloaded versions:
        * `void copyReplaceString(char* dst, size_t dstSize,
          const char* src, char oldChar, const char* newString)`
        * `void copyReplaceString(char* dst, size_t dstSize,
          const char* src, char oldChar,
          const __FlashStringHelper* newString)`
        * `void copyReplaceString(char* dst, size_t dstSize,
          const __FlashStringHelper* src, char oldChar,
          const char* newString)`
        * `void copyReplaceString(char* dst, size_t dstSize,
          const __FlashStringHelper* src, char oldChar,
          const __FlashStringHelper* newString)`

**Print to String Buffer**

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

**Print Utilities**

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
* [src/print_utils/printIntAsFloat.h](src/print_utils/printIntAsFloat.h)
    * Print `uint16_t` and `uint32_t` as a floating point number with 3 decimal
      places after conceptually dividing by 1000.
    * No floating point operations are used.

**Timing Statistics**

* [src/timing_stats/TimingStats.h](src/timing_stats/TimingStats.h)
    * [src/timing_stats/README.md](src/timing_stats/README.md)
    * Helper class to collect data (often durations in milliseconds) and
      then print out various statistics such as min, max, average, and count.
    * `class TimingStats`
* [src/timing_stats/GenericStats.h](src/timing_stats/GenericStats.h)
    * Same as `TimingStats` but templatized to support generic type `T`
    * `class GenericStats<T>`

**Encoding and Decoding**

* [src/url_encoding/url_encoding.h](src/url_encoding/url_encoding.h)
    * [src/url_encoding/README.md](src/url_encoding/README.md)
    * Encodes and decodes strings using "form URL encoding" which converts
        spaces `' '` into `'+'`, and non-alphnumerics into percent-hex digits.
    * `void formUrlEncode(Print& output, const char* str)`
    * `void formUrlDecode(Print& output, const char* str)`
    * `void byteToHexChar(uint8_t c, char* high, char* low, char baseChar =
      'A')`
    * `uint8_t hexCharToByte(char c)`
* [src/backslash_x_encoding/url_encoding.h](src/backslash_x_encoding/url_encoding.h)
    * [src/backslash_x_encoding/README.md](src/backslash_x_encoding/README.md)
    * Encodes non-ASCII printable characters (`c < 32` or `c > 126`) as `\xhh`
      where `hh` is the 2-digit hexadecimal representation of the character. The
      backslash character `\` itself is transformed into 2 backslashes `\\`.
    * `size_t backslashXEncode(char* t, size_t tcap, const char* s, uint8_t*
      status)`
    * `size_t backslashXDecode(char* t, size_t tcap, const char* s, uint8_t*
      status)`

**Hash Code**

* [src/hash/djb2.h](src/hash/djb2.h)
    * The djb2 hash algorithm as described in
      https://stackoverflow.com/questions/7666509 and
      http://www.cse.yorku.ca/~oz/hash.html.
    * `uint32_t hashDjb2(const char* s)`
    * `uint32_t hashDjb2(const __FlashStringHelper* fs)`

**Algorithms**

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

**Version**: 1.6.2 (2023-06-25)

**Changelog**: [CHANGELOG.md](CHANGELOG.md)

**See Also**:

* AceSorting (https://github.com/bxparks/AceSorting)
* AceCRC (https://github.com/bxparks/AceCRC)

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

**Tier 1: Fully supported**

These boards are tested on each release:

* Arduino Nano clone (16 MHz ATmega328P)
* SparkFun Pro Micro clone (16 MHz ATmega32U4)
* Seeeduino XIAO M0 (SAMD21, 48 MHz ARM Cortex-M0+)
* STM32 Blue Pill (STM32F103C8, 72 MHz ARM Cortex-M3)
* Adafruit ItsyBitsy M4 (SAMD51, 120 MHz ARM Cortext-M4)
* NodeMCU 1.0 (ESP-12E module, 80MHz ESP8266)
* WeMos D1 Mini (ESP-12E module, 80 MHz ESP8266)
* ESP32 Dev Module (ESP-WROOM-32 module, 240MHz dual core Tensilica LX6)

**Tier 2: Should work**

These boards should work but I don't test them as often:

* ATtiny85 (8 MHz ATtiny85)
* Arduino Pro Mini clone (16 MHz ATmega328P)
* Mini Mega 2560 (Arduino Mega 2560 compatible, 16 MHz ATmega2560)
* Teensy LC (48 MHz ARM Cortex-M0+)
* Teensy 3.2 (96 MHz ARM Cortex-M4)

**Tier 3: May work, but not supported**

* Other 3rd party SAMD21 boards *may* work if they do not use the
  ArduinoCore-API.

**Tier Blacklisted**

The following boards are *not* supported and are explicitly blacklisted to allow
the compiler to print useful error messages instead of hundreds of lines of
compiler errors:

* Any platform using the ArduinoCore-API
  (https://github.com/arduino/ArduinoCore-api), such as:
    * megaAVR (e.g. Nano Every)
    * SAMD21 boards w/ `arduino:samd` version >= 1.8.10 (e.g. Nano 33 IoT,
      MKRZero, etc)
    * Raspberry Pi Pico RP2040

### Tool Chain

This library was developed and tested using:

* [Arduino IDE 1.8.19](https://www.arduino.cc/en/Main/Software)
* [Arduino CLI 0.31.0](https://arduino.github.io/arduino-cli)
* [Arduino AVR Boards 1.8.6](https://github.com/arduino/ArduinoCore-avr)
* [SparkFun AVR Boards 1.1.13](https://github.com/sparkfun/Arduino_Boards)
* [Arduino SAMD Boards 1.8.9](https://github.com/arduino/ArduinoCore-samd)
* [Adafruit SAMD Boards 1.7.11](https://github.com/adafruit/ArduinoCore-samd)
* [Seeeduino SAMD Boards 1.8.4](https://github.com/Seeed-Studio/ArduinoCore-samd)
* [STM32duino 2.5.0](https://github.com/stm32duino/Arduino_Core_STM32)
* [ESP8266 Arduino Core 3.0.2](https://github.com/esp8266/Arduino)
* [ESP32 Arduino Core 2.0.9](https://github.com/espressif/arduino-esp32)
* [Teensyduino 1.57](https://www.pjrc.com/teensy/td_download.html)

It should work with [PlatformIO](https://platformio.org/) but I have
not tested it.

The library works on Linux or MacOS (using both g++ and clang++ compilers) using
the [EpoxyDuino](https://github.com/bxparks/EpoxyDuino) emulation layer.

### Operating System

I use Ubuntu Linux 22.04 for most of my development.

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
