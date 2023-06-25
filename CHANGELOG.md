# Changelog

* Unreleased
* 1.6.2 (2023-06-25)
    * Add `tstrings.h` which adds various `strxxx_T()` overloaded functions
      which take either a `const char*` or a `const __FlashStringHelper*`
      parameter.
        * Allows writing C++ template code which is agnostic to whether the
          string argument is in normal memory or flash memory. The compiler will
          select the specific implementation automatically at compile-time.
    * `PrintStr.h`
        * Add special exception for ESP32 on PlatformIO which seems to be
          stuck using the ESP32 Core from 1.x, which means that its
          `Print::flush()` is non-virtual instead of virtual (fixed in v2.0.3).
        * See [PR#28](https://github.com/bxparks/AceCommon/pull/28).
* 1.6.1 (2023-06-09)
    * Fix backwards compatibility breakage in `KString()` constructor.
        * v1.5.2 supported only:
            * `KString(cstring, cstring)`
            * `KString(flash, cstring)`
        * v1.6.0 changed to support only (in prep for next version of AceTime):
            * `KString(cstring, cstring)`
            * `KString(flash, flash)`
            * which breaks old version of AceTime v2.2.3
        * v1.6.1 adds all remaining combinations:
            * `KString(flash, cstring)`
            * `KString(cstring, flash)`
        * Now works for both old and new AceTime versions.
* 1.6.0 (2023-06-08)
    * Add overloaded flash string versions of `copyReplaceChar()` and
      `copyReplaceString()` which accept `__FlashStringHelper*` pointers in the
      source string parameters.
    * Support `keywords[]` in PROGMEM in `KString` class.
    * Update supported boards and tiers
        * Add SAMD21 and SAMD51 boards to Tier 1
            * Add 2 SAMD boards from 2 different companies, to test their
              Arduino Board software:
                * Seeeduino XIAO M0 (SAMD21 48MHz ARM Cortex-M0+)
                * Adafruit ItsyBitsy M4 (SAMD51 120MHz ARM Cortex-M4)
            * SAMD21 and SAMD51 boards are back in Tier 1, as long as they use
              the traditional Arduino API instead of the new
              [ArduinoCore-api](https://github.com/arduino/ArduinoCore-api).
            * Fortunately most third party SAMD21 and SAMD51 boards continue to
              use the traditional Arduino API.
        * Move Teensy 3.2 to Tier 2 ("Should work but not tested often")
            * This board is entering end-of-life.
            * As well, the Teensyduino environment integrates with the Arduino
              IDE and CLI in a way that's different than all other third-party
              Arduino boards. Some of my automation scripts do not work with
              Teensyduino, so it becomes very time consuming to test the Teensy
              boards.
            * All Teensy boards are now in Tier 2 .
* 1.5.2 (2022-09-22)
    * Change `hashDjb2(const char*)` to be an inlined function instead of 
      simply delegating to the template version.
        * Increases type-safety because passing an incorrect array (e.g.
          `uint16_t[]`) would compile without any errors, and probably not what
          the user intended since the hash function expects a NUL-terminated
          string.
    * Mark `PrintStr::flush()` as `override`.
        * The STM32 and ESP32 platforms have finally fixed their implementation
          of `Print::flush()` to be `virtual`, consistent with other Arduino
          platforms.
        * Make exception for ATTinyCore, which does not implement a
          `Print::flush()`.
    * Upgrade tool chain
        * Upgrade Arduino CLI from 0.20.2 to 0.27.1.
        * Upgrade Arduino AVR Core from 1.8.4 to 1.8.5.
        * Upgrade STM32 Core from 2.2.0 to 2.3.0.
        * Upgrade ESP32 Core from 2.0.2 to 2.0.5.
        * Upgrade Teensyduino from 1.56 to 1.57.
* 1.5.1 (2022-02-25)
    * **Breaking** Change API of
      [backslash_x_encoding.h](src/backslash_x_encoding) slightly so that the
      `status` is returned and the `written` number of bytes is returned through
      a pointer to an out-parameter.
        * Turns out the more common case is to check the status, and
          ignore the `written`, so the `written` pointer can now be `nullptr`
          if the calling code wishes to ignore it.
        * This is technically a breaking change, but the `backslashXEncode()`
          and `backslashXDecode()` methods were *just* added, but I'm going to
          consider this to be a "quick fix", and increment only the minor
          version number.
* 1.5.0 (2022-02-17)
    * Expose `byteToHexChar()` and `hexCharToByte()` utility functions from
      `url_encoding.h`. They became useful in `backslash_x_encoding.h`.
    * Add [backslash_x_encoding.h](src/backslash_x_encoding) which provides
      `backslashXEncode()` that encodes non-printable ASCII characters to `\xHH`
      form and `backslashXDecode()` to decode back.
    * Upgrade tool chain:
        * Upgrade Arduino IDE from 1.8.16 to 1.8.19.
        * Upgrade Arduino CLI from 0.19.2 to 0.20.2.
        * Upgrade Arduino AVR Core from 1.8.3 to 1.8.4.
        * Upgrade STM32 Core from 2.0.0 to 2.2.0.
        * Upgrade ESP32 Core from 1.0.6 to 2.0.2.
        * Upgrade Teensyduino from 1.55 to 1.56.
    * Downgrade SAMD21 boards to new "Tier 3: May work but unsupported" since I
      can no longer upload to my SAMD21 boards with the current tool chain.
* 1.4.7 (2021-11-04)
    * Add `KString::compareTo(KString&)` using a `KStringIterator`
        * Supports a single level of fragment nesting (i.e. a compression
          fragment cannot contain another fragment).
        * Needed to implement `ZoneSorter` classes in AceTime.
    * Upgrade tool chain:
        * Upgrade Arduino IDE from 1.8.13 to 1.8.16.
        * Upgrade Arduino CLI from 0.14.0 to 0.19.2
        * Upgrade SparkFun SAMD Core from 1.8.3 to 1.8.5.
        * Upgrade ESP8266 Core from 2.7.4 to 3.0.2.
        * Upgrade Teensyduino from 1.53 to 1.55.
* 1.4.6 (2021-08-05)
    * Upgrade STM32duino Core from 1.9.0 to 2.0.0.
        * MemoryBenchmark: Flash usage increases by 2.3kB across the board, but
          static RAM goes down by 250 bytes. Very little change to AceCommon
          code itself.
        * AutoBenchmark: No change.
    * Upgrade SparkFun SAMD Core from 1.8.1 to 1.8.3.
        * No change observed in MemoryBenchmark or AutoBenchmark.
    * Add `print_utils/PrintIntAsFloat.h` with `printUint16AsFloat3To()` and
      `printUint32AsFloat3To()`.
        * Divides the provided value by 1000, then prints the result as a
          floating point number to 3 decimal places.
        * Does not use floating point operations to be friendly on 8-bit
          processors.
    * Add `GenericStats<T>`, a templatized version of `TimingStats`.
    * Update `MemoryBenchmark`
        * Add memory usage for `String`, to provide a better comparison for
          `PrintStr<N>` and `PrintStrN`.
        * `String` consumes about 1000 more flash bytes compared to `PrintStr`.
        * `PrintStrN` consumes about 600 more flash bytes compared to `PrintStr`
          because it pulls in `malloc()` and `free().`
        * Add memory usage for `isSorted()`.
    * Add `isReverseSorted()` and `isReversedSortedByKey()`. Analogous to
      `isSorted()` and `isSortedByKey()`.
* 1.4.5 (2021-04-26)
    * Create more generic `examples/AutoBenchmark` program which replaces
      `examples/Udiv1000`.
    * Activate
      [GitHub Discussions](https://github.com/bxparks/AceCommon/discussions).
    * Add `decToHexDivOnly()` and `decToHexDivMod()` to AutoBenchmark, and
      update `decToHex()` to automatically select the faster one. On AVR,
      `decToHexDivMod()` seems to be 2 clock cycles  (125 ns) faster. But on
      32-bit processors, `decToHexDivOnly()` is 2X faster on those without
      hardware divide (e.g. SAMD21, ESP8266), and 10-20% faster on processors
      with hardware divide (e.g. STM32, ESP32, Teensy 3.2).
    * Prevent overflow of `uint16_t` when calculating exponential decay average.
    * Add `PrintStrBase::cstr()` as a shorter, more convenient version of
      `PrintStrBase::getCstr()`. Retain `getCstr()` for backwards compatibility.
    * Upgrade to ESP32 Core v1.0.6 and regenerate MemoryBenchmark and
      AutoBenchmark.
* 1.4.4 (2021-04-02)
    * Add `copyReplaceChar()` and `copyReplaceString()` functions. Extracted
      from AceTime library.
    * Add `printReplaceCharTo()` and `printReplaceStringTo()` functions.
    * Add [examples/MemoryBenchmark](examples/MemoryBenchmark) to track the
      flash sizes of various components within this library.
    * Add `FlashString` class which is a very thin-wrapper around a
      `const __FlashStringHelper*` pointer, to make it look and act like a
      normal `const char*` pointer.
        * Solves code duplication problem when writing functions that need to
          support both c-strings and f-strings.
        * First write the function against a generic `T` type that is
          assumed to be a type that acts like a `const char*`. Use this for a
          c-string.
        * Second write a template specialization that takes a `const
          __FlashStringHelper*`, wraps the pointer into a `FlashString`,
          and then forwards the call to the generic template function.
* 1.4.3 (2021-02-18)
    * Add `linearSearch()` and `linearSearchByKey()`. Analogs to the
      corresponding binary search functions.
* 1.4.2 (2021-02-13)
    * Add `isSorted()` and `isSortedByKey()` templatized functions which
      determine if an array is sorted or not.
* 1.4.1 (2021-02-11)
    * Simplify `binarySearchByKey()` by removing unnecessary `records`
      parameter. The lambda expression `key` is sufficient.
        * Technically, this is an API breaking change, technically I'm supposed
          bump the minor version number. But the `binarySearchByKey()` feature
          was released only 2 days ago, so let's treat this as a bug fix. In
          other words, the function signature should have been this from the
          beginning.
* 1.4 (2021-02-09)
    * Add `binarySearch()` and `binarySearchByKey()` templatized functions.
    * Explicitly blacklist megaAVR and SAMD21 using `arduino:samd>=1.8.10`
      so that users are given helpful error messages, instead of pages and pages
      of compiler errors. See
      [#8](https://github.com/bxparks/AceCommon/issues/8) and
      [#9](https://github.com/bxparks/AceCommon/issues/9).
* 1.3.1 (2021-01-22)
    * Update UnixHostDuino 0.4 to EpoxyDuino 0.5.
    * No functional change in this release.
* 1.3 (2021-01-19)
    * Add `KString` class that supports string compression using keyword
      substitution.
    * Add support for STM32 by validating on "Blue Pill" board.
    * Update benchmark numbers for `Udiv1000.ino`.
* 1.2 (2021-01-04)
    * Fix `PrintStr.h` for STM32duino by removing `override` in `flush()`.
    * Add `hashDjb2()` hash function for normal and flash strings.
* 1.1.2 (2020-11-28)
    * Minor header include fix for compatibility with UnixHostDuino.
    * Fix various compilation warnings and errors on AVR compiler.
    * No functional change in this release.
* 1.1.1 (2020-11-02)
    * Move `src/fstrings/FCString.h` from AceRoutine to AceCommon library.
    * Fix typos and broken links in README.md files and example code.
* 1.1 (2020-10-29)
    * Split `src/ace_common` to `src/arithmetic` and `src/pstrings`.
    * Move `src/print_str` from AceUtils to AceCommon library.
    * Move `src/print_utils` from AceUtils to AceCommon library.
    * Move `src/timing_stats` from AceUtils to AceCommon library.
    * Move `src/url_encoding` from AceUtils to AceCommon library.
* 1.0 (2020-10-20)
    * Add `src/ace_common/arithmetic.h` and unit tests
    * Add `src/ace_common/pstrings.h` and unit tests
    * Add `udiv1000()` to `arithmetic.h`
    * Add GitHub Actions workflow for continuous integration and testing.
