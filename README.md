# AceCommon

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
    * `incrementMod(c, m)`
    * `incrementModOffset(c, m, offset)`
    * `decToBcd(uint8_t)`
    * `bcdToDec(uint8_t)`
    * `udiv1000(unsigned long)`
* `src/pstrings/pstrings.h`
    * `strcmp_PP(a, b)`
    * `strchr_P(a, b)` (ESP8266 and ESP32 only)
    * `strrchr_P(a, b)` (ESP8266 and ESP32 only)
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
    * `printPad2To(Print& printer, uint16_t val, char pad = ' ')`
    * `printPad3To(Print& printer, uint16_t val, char pad = ' ')`
    * `printPad4To(Print& printer, uint16_t val, char pad = ' ')`
    * `printPad5To(Print& printer, uint16_t val, char pad = ' ')`
* `src/print_utils.printfTo.h`
    * [src/print_utils/README.md](src/print_utils/README.md)
        * Provides a primitive `printf()` functionality to an instance of
          `Print` (e.g. `Serial`) for those Arduino boards without a
          `Print.printf()` function.
    * `printfTo(Print& printer, const char* fmt, ...)`
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
    * `formUrlEncode(Print& output, const char* str)`
    * `formUrlDecode(Print& output, const char* str)`
* `src/fstrings/FCString.h`
    * `class FCString;`
    * An object that can hold either a C-string (`const char*`) or an
      F-string (`const __FlashStringHelper*`).

**Version**: 1.1.1 (2020-11-02)

**Changelog**: [CHANGELOG.md](CHANGELOG.md)

![AUnit Tests](https://github.com/bxparks/AceCommon/workflows/AUnit%20Tests/badge.svg)

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

* UnixHostDuino (https://github.com/bxparks/UnixHostDuino)

### Source Code

The source files are organized as follows:

* `src/AceCommon.h` - main header file
* `src/*/` - implementation files
* `tests/` - unit tests which require [AUnit](https://github.com/bxparks/AUnit)
* `examples/` - example sketches

### Docs

Besides this README.md file, the [docs/](docs/) directory contains the Doxygen
docs (https://bxparks.github.io/AceCommon/html/) published on GitHub Pages. It
can help you navigate an unfamiliar code base.

## License

[MIT License](https://opensource.org/licenses/MIT)

## Feedback and Support

If you have any questions, comments, bug reports, or feature requests, please
file a GitHub ticket instead of emailing me unless the content is sensitive.
(The problem with email is that I cannot reference the email conversation when
other people ask similar questions later.) I'd love to hear about how this
software and its documentation can be improved. I can't promise that I will
incorporate everything, but I will give your ideas serious consideration.

## Author

Created by Brian T. Park (brian@xparks.net).
