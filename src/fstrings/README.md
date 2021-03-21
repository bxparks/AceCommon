# Flash Strings

These are utilities and classes for working with strings stored in flash (`const
__FlashStringHelper*`). On certain chips (AVR, ESP8266), storing strings in
flash memory using the `PROGMEM` keyword and the `F()` macro can reduce static
RAM consumption. On most other 32-bit processors, const strings are
automatically stored in flash memory, and the `PROGMEM` and `F()` are defined
for compatibility, but don't do anything.

## FlashString Class

This is a very thin wrapper class that looks and acts like a normal c-string
pointer (`const char*`). Its purpose is to help avoid duplication of code. When
writing string manipulation utilties, it is often useful to provide variants of
the utilities that accept a flash string with the type `const
__FlashStringHelper*` in addition to the normal c-string `const char*`. But
accessing these flash pointers must be done with the `pgm_read_byte()` method,
instead of just dereferencing the `const char*` pointer with a `*` operator.
This usually means that we have to write out the algorithm twice, with almost
duplicate code, except for some minor variations.

The `FlashString` class works by wrapping the `const __FlashStringHelper*`
pointer into a `FlashString` object which acts basically like a `const char*`
pointer. You can then write a generic template function that handles the normal
`const char*` pointer case, then reuse the same function when called with the
`const __FlashStringHelper*` pointer.

The `FlashString` implements various methods to emulate a `const char*`, with
the critical `operator*()` method calling the `pgm_read_byte()` function:

```C++
namespace ace_common {

class FlashString {
  public:
    FlashString(const __FlashStringHelper* p)
        : fsp((const char*) p) {}

    char operator*() const { return (char) pgm_read_byte(fsp); }

    operator const __FlashStringHelper*() const;

    FlashString operator++();

    FlashString operator++(int);

    FlashString operator--();

    FlashString operator--(int);
};

}
```

Then for your code, write out the template function using a generic `T` class
while assuming that it acts like a simple `const char*` pointer:

```C++
#include <AceCommon.h>
using ace_common::FlashString;

...

// Generic template function, suitable for (const char*).
template <typename T>
void someStringFunction(..., T cs, ...) {
  // do work with the 'cs' pointer
}

// Template specialization for (const __FlashStringHelper*). The implementation
// wraps the `__FlashStringHelper` pointer inside a FlashString, and then
// forwards to the generic template function.
template <>
void someStringFunction<const __FlashStringHelper*>(
    ..., const __FlashStringHelper* fs, ...) {

  someStringFunction<FlashString>(..., FlashString(cs), ...);
}
```

See [print_utils/printReplaceTo.h](../print_utils/printReplaceTo.h) for an
example of this in action.

## FCString Class

This class stores *either* a c-string (`const char*`) or a flash string (`const
__FlashStringHelper*`) in a single object, and provides a type discriminator
that indicates which type of string is being stored. The original pointer can be
retrieved using a `getCString()` or a `getFString()`.

```C++
namespace ace_common {

class FCString {
  public:
    static const uint8_t kCStringType = 0;
    static const uint8_t kFStringType = 1;

    FCString() {}

    explicit FCString(const char* s);

    explicit FCString(const __FlashStringHelper* s);

    uint8_t getType() const;

    const char* getCString() const;

    const __FlashStringHelper* getFString() const;

    size_t printTo(Print& printer) const;

    int compareTo(const FCString& that) const;
};

}
```
