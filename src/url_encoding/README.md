# Url Encoding

Quick and dirty implementation of the URL encoding used for HTML forms:

* See https://en.wikipedia.org/wiki/Percent-encoding and
* https://stackoverflow.com/questions/1634271.

Quick summary of Form URL encoding:

* Alpha numeric `[a-zA-Z0-9]` are preserved,
* Spaces are converted to `+`,
* all other characters are percent-encoded (`%` followed by the 2-digit
  hexademimal value of the character)

Two functions are provided:

* `void formUrlEncode(Print& output, const char* str);`
* `void formUrlDecode(Print& output, const char* str);`

These functions do *not* use the `String` class so that we can avoid heap
fragmentation associated with numerous creation and deletion of small `String`
objects. Instead, these functions print to the `output` parameter implementing
the `Print` interface.

The most convenient implementation of the `Print` interface is one of the [Print
String](../print_str) classes provided in this library. These classes allow the
encoded or decoded string to be printed to an in-memory buffer. The `PrintStr`
or `PrintStrN` object can be converted into a normal c-string (`const char*`)
using the `cstr()` method.

You can also pass a `Serial` object as the `output` if that is more
convenient, since `Serial` also implements the `Print` interface.

## Usage

```C++
#include <Arduino.h>
#include <AceCommon.h>
using namespace ace_common;

const char MESSAGE[] = "0aA %";

const char ENCODED_MESSAGE[] = "0aA+%25";

void encodeToSerial() {
  formUrlEncode(Serial, MESSAGE);
  Serial.println();
}

void encodeToMemory() {
  PrintStr<20> printString;
  formUrlEncode(printString, MESSAGE);
  Serial.println(printString.cstr());
}

void decodeToSerial() {
  formUrlDecode(Serial, ENCODED_MESSAGE);
  Serial.println();
}

void decodeToMemory() {
  PrintStr<20> printString;
  formUrlDecode(printString, ENCODED_MESSAGE);
  Serial.println(printString.cstr());
}

void setup() {
  Serial.begin(115200);
  ...
}

void loop() {
  ...
}
```

## Source Code History

These routines were inspired by:

* https://github.com/TwilioDevEd/twilio_esp8266_arduino_example
* https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino.

The original code was rewritten to create the functions in this library in the
following way:

* Fix buffer overrun bug for decoding strings which are malformed.
* Apply consistent style and code formatting.
* Use the `PrintStr` object instead of `String` to avoid repeated
allocation of small strings which causes heap fragmentation.
* Remove the calls to `yield()` on every iteration because using `PrintStr`
makes this implementation 5-6 times faster, so there is little danger of
triggering the Watchdog Timer reset on a ESP8266. (See below).

## Avoiding the Watchdog Timer Reset

The original `urlencode()` and `urldecode()` methods from
https://github.com/TwilioDevEd/twilio_esp8266_arduino_example call the `yield()`
function on every iteration of the loop. I assume that this was inserted to
avoid triggering the Watchdog Timer (WDT) reset on large strings on ESP8266
processors. If a single function consumes more than 20-40 milliseconds without
calling the `yield()` function on an ESP8266, the processor is automatically
reset by the WDT.

This code in this library uses `PrintStr` class instead of a `String` class.
Performance benchmarks at
[examples/UrlEncodingBenchmark](../examples/UrlEncodingBenchmark/)
show that the new code is 5-6 times faster than the old code on an ESP8266. For
example, the `formUrlEncode()` function takes about 1500 microseconds per 1000
characters, compared to 7700 microseconds per 1000 characters in the original
code. The `formUrlDecode()` function takes about 910 microseconds per 1000
characters, compared to 5300 microseconds for the old code.

With this increased performance, it is possible to encode and decode strings as
large as 10000 characters long within 15 milliseconds, which is fast enough to
avoid triggering the WDT reset. Therefore, I decided to remove the calls
`yield()` function, because I do not see myself needing these functions for
strings longer than 10,000 characters.

On a dual-core ESP32, my understanding is that it is not necessary to call the
`yield()` function periodically, so the functions in this library should work
fine.

If the `yield()` function needs to be inserted into these functions on an
ESP8266 to handle very long strings, I recommend using a counter so that the
`yield()` is called every 500-1000 iterations (for example) instead of
calling it on every iteration.
