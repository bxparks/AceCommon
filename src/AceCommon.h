/*
MIT License

Copyright (c) 2020 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * @mainpage AceCommon Library
 *
 * This is the Doxygen documentation for the
 * <a href="https://github.com/bxparks/AceCommon">AceCommon Library</a>.
 *
 * Click on the "Classes" menu above to see the list of classes.
 *
 * Click on the "Files" menu above to see the list of header files.
 */

#ifndef ACE_COMMON_H
#define ACE_COMMON_H

// Blacklist platforms using https://github.com/arduino/ArduinoCore-api due to
// incompatibilities. This currently includes all megaAVR boards and SAMD21
// boards using arduino::samd >= 1.8.10.
#if defined(ARDUINO_API_VERSION)
#error Platforms using ArduinoCore-API not supported
#endif

#include "arithmetic/arithmetic.h"

#include "pstrings/pstrings.h"
#include "fstrings/FCString.h"
#include "fstrings/FlashString.h"
#include "kstrings/KString.h"
#include "tstrings/tstrings.h"
#include "cstrings/copyReplace.h"

#include "print_str/PrintStr.h"

#include "print_utils/printPadTo.h"
#include "print_utils/printfTo.h"
#include "print_utils/printReplaceTo.h"
#include "print_utils/printIntAsFloat.h"

#include "timing_stats/TimingStats.h"
#include "timing_stats/GenericStats.h"

#include "url_encoding/url_encoding.h"
#include "backslash_x_encoding/backslash_x_encoding.h"

#include "hash/djb2.h"

#include "algorithms/binarySearch.h"
#include "algorithms/isSorted.h"
#include "algorithms/linearSearch.h"
#include "algorithms/reverse.h"

// Version format: "xx.yy.zz" => xxyyzz (without leading 0)
#define ACE_COMMON_VERSION 10602
#define ACE_COMMON_VERSION_STRING "1.6.2"

#endif
