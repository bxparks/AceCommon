# Changelog

* Unreleased
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
