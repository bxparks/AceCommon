# Print Utils

Here are some utilities to make printing easier.

## Printf To Printer

Many Arduino platforms (ESP8266, ESP32, TEENSYDUINO) implement a `printf()`
method on the `Print` interface, which allows you to write something like:

```C++
int count = 10;
Serial.printf("There are %d items\n", count);
```

Many other Arduino platforms (e.g. UNO or Nano) do not provide a
`Print.printf()` function. The `printfTo()` function in this library provides a
workaround.

Usage:

```C++
#include <AceCommon.h>
using namespace ace_common;

void print() {
  int count = 10;
  printfTo(Serial, "There are %d items\n", count);
}

[...]
```

This will print:
```
There are 10 items
```

An internal buffer of 192 characters is created temporarily on the stack, which
limits the output to 191 characters (excluding the terminating `'\0'`
character). The internal compile-time constant `print_utils::BUF_SIZE` can be
adjusted smaller or larger if necessary.

Internally, the `vsnprintf()` function is used to implement the `printf()`
functionality, which will cause the program size to grow substantially. You will
likely want to use `printfTo()` only for debugging purposes.

## Print with Padding To Printer

Print unsigned integers with left padding using the least amount of code. The
padding can be user-defined and defaults to a space ' ' character. This can be
be a low-memory alternative to using `Serial.printf()` or `printfTo()`.

Usage:

```C++
#include <AceCommon.h>
using namespace ace_common;

void printPadSpaces() {
  uint16_t count = 9;

  printPad2To(Serial, count); // prints " 9"
  printPad3To(Serial, count); // prints "  9"
  printPad4To(Serial, count); // prints "   9"
  printPad5To(Serial, count); // prints "    9"
}

void printPadZeros(){
  uint16_t count = 9;

  printPad2To(Serial, count, '0'); // prints "09"
  printPad3To(Serial, count, '0'); // prints "009"
  printPad4To(Serial, count, '0'); // prints "0009"
  printPad5To(Serial, count, '0'); // prints "00009"
}
```

This is useful for printing fixed point numbers, without having to use the
`float` type.

```C++
void printTemp(uint16_t centiDegrees) {
  uint16_t whole = centiDegrees / 100;
  uint16_t fraction = centiDegrees - whole * 100;
  Serial.print(whole);
  Serial.print('.');
  printPad2To(Serial, fraction, '0');
  Serial.print('C');
}

// Prints "13.05C".
uint16_t centiDegrees = 1305; // temperature in units of 0.01 C
printTemp(centiDegrees);
```

Another situation where `printfTo()` is useful is printing rows of numbers in a
tabular format with fixed-width columns.
