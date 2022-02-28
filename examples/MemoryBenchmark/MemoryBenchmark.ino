/*
 * Determine the size of various components of the AceCommon library.
 */

#include <stdint.h> // uint8_t
#include <Arduino.h>
#include <AceCommon.h>
using namespace ace_common;

// List of features of the AceCommon library that we want to examine.
#define FEATURE_BASELINE 0
#define FEATURE_STRING 1
#define FEATURE_PRINT_STR 2
#define FEATURE_PRINT_STR_N 3
#define FEATURE_PRINT_PAD_2_TO 4
#define FEATURE_PRINT_PAD_5_TO 5
#define FEATURE_PRINT_UINT16_AS_FLOAT3_TO 6
#define FEATURE_PRINT_UINT32_AS_FLOAT3_TO 7
#define FEATURE_PRINT_REPLACE_CHAR_TO_CSTRING 8
#define FEATURE_PRINT_REPLACE_CHAR_TO_FSTRING 9
#define FEATURE_PRINT_REPLACE_STRING_TO_CSTRING 10
#define FEATURE_PRINT_REPLACE_STRING_TO_FSTRING 11
#define FEATURE_HASH_DJB2_CSTRING 12
#define FEATURE_HASH_DJB2_FSTRING 13
#define FEATURE_UDIV_1000 14
#define FEATURE_NATIVE_DIV_1000 15
#define FEATURE_IS_SORTED 16
#define FEATURE_REVERSE 17
#define FEATURE_KSTRING_COMPARE_TO_CSTRING 18
#define FEATURE_KSTRING_COMPARE_TO_KSTRING 19
#define FEATURE_BACKSLASH_X_ENCODE 20
#define FEATURE_BACKSLASH_X_DECODE 21

// Select one of the FEATURE_* parameter and compile. Then look at the flash
// and RAM usage, compared to FEATURE_BASELINE usage to determine how much
// flash and RAM is consumed by the selected feature.
// NOTE: This line is modified by a 'sed' script in collect.sh. Be careful
// when modifying its format.
#define FEATURE 0

// Define SERIAL_PORT_MONITOR for ESP32
#ifndef SERIAL_PORT_MONITOR
  #define SERIAL_PORT_MONITOR Serial
#endif

// Set this variable to prevent the compiler optimizer from removing the code
// being tested when it determines that it does nothing.
volatile uint8_t guard;

// Temporary array used by some functions below, included in Baseline so that
// it is removed from the actual benchmark.
const uint8_t ARRAY_SIZE = 100;
uint8_t array[ARRAY_SIZE];
static void fillArray(uint8_t array[], uint16_t n) {
  for (uint16_t i = 0; i < n; ++i) {
    array[i] = random(256);
  }
}

// Some strings used by encode() and decode() functions below, included in
// Baseline so that it is removed from actual benchmark.
const char DECODED_STRING[] = "\n\tab\\";
const char ENCODED_STRING[] = R"(\x0D\x09ab\)";

void setup() {
  // Setup the Serial, to force inclusion of the Print class in the Baseline, so
  // that we can measure the incremental amount of code brought in by PrintStr<>
  // and PrintStrN<>.
  SERIAL_PORT_MONITOR.begin(115200);
  while (! SERIAL_PORT_MONITOR); // Leonardo/Micro
  SERIAL_PORT_MONITOR.print(guard);

  // Prevent compiler from optimizing away various temp arrays.
  fillArray(array, ARRAY_SIZE);
  array[ARRAY_SIZE - 1] = '\0';
  guard = array[array[0]];
  guard = ENCODED_STRING[3];
  guard = DECODED_STRING[4];

#if FEATURE == FEATURE_BASELINE
  // nothing
#elif FEATURE == FEATURE_STRING
  String s;
  s.concat(guard);
  guard = s.c_str()[0];
#elif FEATURE == FEATURE_PRINT_STR
  PrintStr<16> printStr;
  printStr.print(guard);
  guard = printStr.cstr()[0];
#elif FEATURE == FEATURE_PRINT_STR_N
  PrintStrN printStr(16);
  printStr.print(guard);
  guard = printStr.cstr()[0];
#elif FEATURE == FEATURE_PRINT_PAD_2_TO
  printPad2To(SERIAL_PORT_MONITOR, 1);
#elif FEATURE == FEATURE_PRINT_PAD_5_TO
  printPad5To(SERIAL_PORT_MONITOR, 1);
#elif FEATURE == FEATURE_PRINT_UINT16_AS_FLOAT3_TO
  printUint16AsFloat3To(SERIAL_PORT_MONITOR, (uint16_t) 123);
#elif FEATURE == FEATURE_PRINT_UINT32_AS_FLOAT3_TO
  printUint32AsFloat3To(SERIAL_PORT_MONITOR, (uint32_t) 123456UL);
#elif FEATURE == FEATURE_PRINT_REPLACE_CHAR_TO_CSTRING
  printReplaceCharTo(SERIAL_PORT_MONITOR, "E%T", '%', 'S');
#elif FEATURE == FEATURE_PRINT_REPLACE_CHAR_TO_FSTRING
  printReplaceCharTo(SERIAL_PORT_MONITOR, F("E%T"), '%', 'S');
#elif FEATURE == FEATURE_PRINT_REPLACE_STRING_TO_CSTRING
  printReplaceStringTo(SERIAL_PORT_MONITOR, "E%T", '%', "S");
#elif FEATURE == FEATURE_PRINT_REPLACE_STRING_TO_FSTRING
  printReplaceStringTo(SERIAL_PORT_MONITOR, F("E%T"), '%', "S");
#elif FEATURE == FEATURE_HASH_DJB2_CSTRING
  uint32_t hash = hashDjb2("1234");
  guard = hash;
#elif FEATURE == FEATURE_HASH_DJB2_FSTRING
  uint32_t hash = hashDjb2(F("1234"));
  guard = hash;
#elif FEATURE == FEATURE_UDIV_1000
  uint32_t value = (uint32_t) guard << 24
      | ((uint32_t) guard << 16)
      | (guard << 8)
      | guard;
  value = udiv1000(value);
  guard = value;
#elif FEATURE == FEATURE_NATIVE_DIV_1000
  uint32_t value = (uint32_t) guard << 24
      | ((uint32_t) guard << 16)
      | (guard << 8)
      | guard;
  value = value / 1000;
  guard = value;

#elif FEATURE == FEATURE_IS_SORTED
  bool sorted = isSorted(array, ARRAY_SIZE);
  guard = sorted;

#elif FEATURE == FEATURE_REVERSE
  reverse(array, ARRAY_SIZE);
  guard = array[0];

#elif FEATURE == FEATURE_KSTRING_COMPARE_TO_CSTRING
  const int NUM_KEYWORDS = 2;
  const char* const KEYWORDS[NUM_KEYWORDS] = {
    nullptr,
    "Africa/", // \x01
  };
  KString k1("\x01" "Cairo", KEYWORDS, NUM_KEYWORDS);
  int cmp = k1.compareTo("Africa/Ceuta");
  guard = cmp;

#elif FEATURE == FEATURE_KSTRING_COMPARE_TO_KSTRING
  const int NUM_KEYWORDS = 2;
  const char* const KEYWORDS[NUM_KEYWORDS] = {
    nullptr,
    "Africa/", // \x01
  };
  KString k1("\x01" "Cairo", KEYWORDS, NUM_KEYWORDS);
  KString k2("\x01" "Ceuta", KEYWORDS, NUM_KEYWORDS);
  int cmp = k1.compareTo(k2);
  guard = cmp;

#elif FEATURE == FEATURE_BACKSLASH_X_ENCODE
  size_t written;
  uint8_t status = backslashXEncode(
      (char*) array, sizeof(array), DECODED_STRING, &written);
  (void) status;
  guard = array[2];

#elif FEATURE == FEATURE_BACKSLASH_X_DECODE
  size_t written;
  uint8_t status = backslashXDecode(
      (char*) array, sizeof(array), ENCODED_STRING, &written);
  (void) status;
  guard = array[2];

#else
  #error Unknown FEATURE
#endif
}

void loop() {
}
