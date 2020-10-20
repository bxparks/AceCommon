# AceCommon

An Arduino library containing small, low-level functions and routines which have
no dependencies to any other external libraries so that they can be easily
reused from other Arduino libraries in my collection.

All features are accessible through a single include file, using the
`ace_common` namespace:
```
#include <AceCommon.h>
using namespace ace_common;
```

The `AceCommon.h` header file includes the following sub-header files
automatically:

* `arithmetic.h`
    * `incrementMod(c, m)`
    * `incrementModOffset(c, m, offset)`
    * `decToBcd()`
    * `bcdToDec()`
    * `udiv1000()`
* `pstrings.h`
    * `strcmp_PP(a, b)`
    * `strchr_P(a, b)` (ESP8266 and ESP32 only)
    * `strrchr_P(a, b)` (ESP8266 and ESP32 only)

**Version**: 1.0 (2020-10-19)

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

The unit tests under `tests/` depends on:

* AUnit (https://github.com/bxparks/AUnit)

The unit tests can be executed on Linux or MacOS using:

* UnixHostDuino (https://github.com/bxparks/UnixHostDuino)

### Source Code

The source files are organized as follows:

* `src/AceCommon.h` - main header file
* `src/ace_common/` - implementation files
* `tests/` - unit tests which require [AUnit](https://github.com/bxparks/AUnit)
* `examples/` - example sketches

### Docs

Besides this README.md file, the [docs/](docs/) directory contains the Doxygen
docs (https://bxparks.github.io/AceCommon/html/) published on GitHub Pages. It
can help you navigate an unfamiliar code base.

## Usage

All functions and classes are available using a single header file,
under the `ace_common` namespace:

```
#include <AceCommon.h>
using namespace ace_common;
...
```

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
