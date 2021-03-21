/*
 * Determine the size of various components of the AceCommon library.
 */

#include <stdint.h> // uint8_t
#include <Arduino.h>
#include <AceCommon.h>
using namespace ace_common;

// List of features of the AceCommon library that we want to examine.
#define FEATURE_BASELINE 0
#define FEATURE_PRINT_STR_N 1
#define FEATURE_PRINT_PAD_2_TO 2
#define FEATURE_PRINT_PAD_5_TO 3
#define FEATURE_PRINT_REPLACE_CHAR_TO 4
#define FEATURE_PRINT_REPLACE_CHAR_FLASH_TO 5
#define FEATURE_PRINT_REPLACE_STRING_TO 6
#define FEATURE_PRINT_REPLACE_STRING_FLASH_TO 7

// Select one of the FEATURE_* parameter and compile. Then look at the flash
// and RAM usage, compared to FEATURE_BASELINE usage to determine how much
// flash and RAM is consumed by the selected feature.
// NOTE: This line is modified by a 'sed' script in collect.sh. Be careful
// when modifying its format.
#define FEATURE 0

// Set this variable to prevent the compiler optimizer from removing the code
// being tested when it determines that it does nothing.
volatile uint8_t guard;

void setup() {
#if FEATURE == FEATURE_BASELINE
  PrintStr<16> printStr;
  printStr.write(' ');
  guard = printStr.getCstr()[0];
#elif FEATURE == FEATURE_PRINT_STR_N
  PrintStrN printStr(16);
  printStr.write(' ');
  guard = printStr.getCstr()[0];
#elif FEATURE == FEATURE_PRINT_PAD_2_TO
  PrintStr<16> printStr;
  printPad2To(printStr, 1);
  guard = printStr.getCstr()[1];
#elif FEATURE == FEATURE_PRINT_PAD_5_TO
  PrintStr<16> printStr;
  printPad5To(printStr, 1);
  guard = printStr.getCstr()[0];
#elif FEATURE == FEATURE_PRINT_REPLACE_CHAR_TO
  PrintStr<16> printStr;
  printReplaceCharTo(printStr, "E%T", '%', 'S');
  guard = printStr.getCstr()[2];
#elif FEATURE == FEATURE_PRINT_REPLACE_CHAR_FLASH_TO
  PrintStr<16> printStr;
  printReplaceCharTo(printStr, F("E%T"), '%', 'S');
  guard = printStr.getCstr()[2];
#elif FEATURE == FEATURE_PRINT_REPLACE_STRING_TO
  PrintStr<16> printStr;
  printReplaceStringTo(printStr, "E%T", '%', "S");
  guard = printStr.getCstr()[2];
#elif FEATURE == FEATURE_PRINT_REPLACE_STRING_FLASH_TO
  PrintStr<16> printStr;
  printReplaceStringTo(printStr, F("E%T"), '%', "S");
  guard = printStr.getCstr()[2];
#else
  #error Unknown FEATURE
#endif
}

void loop() {
}
