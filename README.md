# AceCommon

![AUnit Tests](https://github.com/bxparks/AceCommon/workflows/AUnit%20Tests/badge.svg)

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

* `src/arithmetic/arithmetic.h`
    * `void incrementMod(T& c, T m)`
    * `void incrementModOffset(T&c, T m, T offset)`
    * `uint8_t decToBcd(uint8_t)`
    * `uint8_t bcdToDec(uint8_t)`
    * `unsigned long udiv1000(unsigned long)`
* `src/pstrings/pstrings.h`
    * `int strcmp_PP(const char* a, const char* b)`
    * `const char* strchr_P(const char* s, char c)` (ESP8266 and ESP32 only)
    * `const char* strrchr_P(const char* s, char c)` (ESP8266 and ESP32 only)
* `src/print_str/PrintStr.h`
    * [src/print_str/README.md](src/print_str/README.md)
        * Provides classes that implement the `Print` interface so that
          values can be printed into in-memory buffers. The string can then
          be extracted as a normal c-string (`const char*`).
        * Alternative to the Arduino `String` class to avoid or reduce heap
          fragmentation.
    * `class PrintStrBase`
    * `class PrintStr<uint16_t SIZE>` (buffer on stack)
    * `class PrintStrN(uint16_t size)` (buffer on heap)
* `src/print_utils/printPadTo.h`
    * [src/print_utils/README.md](src/print_utils/README.md)
    * `void printPad2To(Print& printer, uint16_t val, char pad = ' ')`
    * `void printPad3To(Print& printer, uint16_t val, char pad = ' ')`
    * `void printPad4To(Print& printer, uint16_t val, char pad = ' ')`
    * `void printPad5To(Print& printer, uint16_t val, char pad = ' ')`
* `src/print_utils.printfTo.h`
    * [src/print_utils/README.md](src/print_utils/README.md)
        * Provides a primitive `printf()` functionality to an instance of
          `Print` (e.g. `Serial`) for those Arduino boards without a
          `Print.printf()` function.
    * `void printfTo(Print& printer, const char* fmt, ...)`
* `src/timing_stats/TimingStats.h`
    * [src/timing_stats/README.md](src/timing_stats/README.md)
        * Helper class to collect data (often durations in milliseconds) and
          then print out various statistics such as min, max, average, and
          count.
    * `class TimingStats`
* `src/url_encoding/url_encoding.h`
    * [src/url_encoding/README.md](src/url_encoding/README.md)
        * Encodes and decodes strings using "form URL encoding" which converts
          spaces `' '` into `'+'`, and non-alphnumerics into percent-hex digits.
    * `void formUrlEncode(Print& output, const char* str)`
    * `void formUrlDecode(Print& output, const char* str)`
* `src/fstrings/FCString.h`
    * `class FCString`
    * An object that can hold either a C-string (`const char*`) or an
      F-string (`const __FlashStringHelper*`).
* `src/hash/djb2.h`
    * The djb2 hash algorithm as described in
      https://stackoverflow.com/questions/7666509 and
      http://www.cse.yorku.ca/~oz/hash.html.
    * `uint32_t hashDjb2(const char* s);`
    * `uint32_t hashDjb2(const __FlashStringHelper* fs);`
* `src/kstrings/KString.h`
    * Wrapper around a c-string or an f-string which supports compression
      using keyword substitution from a dictionary. Up to 31 keywords supported.
    * `class KString`

**Version**: 1.3.1 (2021-01-22)

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

### Tool Chain

This library was developed and tested using:

* [Arduino IDE 1.8.13](https://www.arduino.cc/en/Main/Software)
* [Arduino CLI 0.14.0](https://arduino.github.io/arduino-cli)
* [Arduino AVR Boards 1.8.3](https://github.com/arduino/ArduinoCore-avr)
* [Arduino SAMD Boards 1.8.9](https://github.com/arduino/ArduinoCore-samd)
* [SparkFun AVR Boards 1.1.13](https://github.com/sparkfun/Arduino_Boards)
* [SparkFun SAMD Boards 1.8.1](https://github.com/sparkfun/Arduino_Boards)
* [STM32duino 1.9.0](https://github.com/stm32duino/Arduino_Core_STM32)
* [ESP8266 Arduino Core 2.7.4](https://github.com/esp8266/Arduino)
* [ESP32 Arduino Core 1.0.4](https://github.com/espressif/arduino-esp32)
* [Teensyduino 1.53](https://www.pjrc.com/teensy/td_download.html)

It should work with [PlatformIO](https://platformio.org/) but I have
not tested it.

The library works on Linux or MacOS (using both g++ and clang++ compilers) using
the [EpoxyDuino](https://github.com/bxparks/EpoxyDuino) emulation layer.

### Operating System

I use Ubuntu Linux 18.04 and 20.04 for most of my development.

## License

[MIT License](https://opensource.org/licenses/MIT)

## Feedback and Support

If you find this library useful, consider starring this project on GitHub. The
stars will let me prioritize the more popular libraries over the less popular
ones.

If you have any questions, comments, bug reports, or feature requests, please
file a GitHub ticket instead of emailing me unless the content is sensitive.
(The problem with email is that I cannot reference the email conversation when
other people ask similar questions later.) I'd love to hear about how this
software and its documentation can be improved. I can't promise that I will
incorporate everything, but I will give your ideas serious consideration.

## Author

Created by Brian T. Park (brian@xparks.net).
