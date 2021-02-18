# Changelog

* Unreleased
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
    * Minor header include fix for compatility with UnixHostDuino.
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
