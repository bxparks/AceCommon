# Backslash X Encoding

Quick and simple implementation of the backslash-x encoding `\xHH`, where `HH`
are 2 hexadecimal digits. This is intended to be used to view ASCII strings with
embedded control characters as printable characters, without the danger of
messing up the character terminal due to the control characters.

* See [Python PEP223](https://www.python.org/dev/peps/pep-0223/)

Quick summary of the encoding:

* Printable ASCII characters (32-127) are preserved, except for the backslash.
* Non-printable characters are converted into `\xHH`, where `HH` is the
  2-digit hexadecimal ASCII code.
* Special conversion: Backslash `\` itself is converted into two backslashes
  `\\` instead of `\x5A` for readability.

Two functions are provided:

```C++
uint8_t backslashXEncode(char* t, size_t tcap, const char* s, uint8_t* written);
uint8_t backslashXDecode(char* t, size_t tcap, const char* s, uint8_t* written);
```

* The source `s` and target `t` are both always NUL-terminated C-strings.
* The `tcap` is the capacity of `t`, including the NUL termination character.
* The `written` parameter is a pointer to an output parameter to indicate
* The `written` parameter is the number of bytes written to `t`. A `nullptr`
  can be passed in if you don't care about this. It is intended to be helpful
  for chaining multiple calls into a single target `t` string.
* The return value is a status code where 0 means success, and anything else is
  an error.

These functions do *not* use the `String` class to avoid heap fragmentation.

## Usage

```C++
#include <Arduino.h>
#include <AceCommon.h>
using namespace ace_common;

const char ORIGINAL_MESSAGE[] = "hello\r\nthere\\";

const char ENCODED_MESSAGE[] = R"(hello\x0A\x0Dthere\\)";

void encode() {
  char t[64];
  size_t written;
  uint8_t status = backslashXEncode(t, sizeof(t), ORIGINAL_MESSAGE, &written);
  if (status) {
    Serial.println("ERROR");
    ...
  }

  // Prints "hello\x0A\x0Dthere\\"
  Serial.println(t);
}

void decode() {
  char t[64];
  size_t written;
  uint8_t status = backslashXDecode(t, sizeof(t), ENCODED_MESSAGE, &written);
  if (status) {
    Serial.println("ERROR");
    ...
  }

  // Prints "hello" on one line, "there\" on the next line.
  Serial.println(t);
}

void setup() {
  Serial.begin(115200);
  ...
}

void loop() {
  ...
}
```

See [tests/BackslashXEncodingTest](../../tests/BackslashXEncodingTest/) for
examples of error handling.
