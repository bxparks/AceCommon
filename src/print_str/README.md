# Print String

An implementation of `Print` that writes to an in-memory buffer supporting
strings less than 65535 in length. It is intended to be an alternative to the
`String` class to help avoid heap fragmentation due to repeated creation and
deletion of small String objects. The `PrintStr` object inherit the methods
from the `Print` interface which can be used to build an internal string
representation of various objects. Instead of using the `operator+=()` or the
`concat()` method, use the `print()`, `println()` (or sometimes the `printf()`
method) of the `Print` class. After the internal string is built, the
NUL-terminated c-string representation can be retrieved using the `cstr()`
method (previously known as `getCstr()` which is retained for backwards
compatibility).

## Usage

There are 2 implementations of `PrintStr`:

* `PrintStr<uint16_t SIZE>` is a templatized class where the character array
  buffer is created on the stack.
    * The `SiZE` parameter is a compile-time constant and given as a template
      parameter.
    * The largest `SIZE` is limited by the amount of stack space available,
      which can be much smaller than the total amount of static RAM. For
      example, on the ESP8266, the maximum stack size is about 4kB, even though
      the total static RAM is 80kB.
    * In practice, `SIZE` should probably be less than about 400-500.
    * This version is recommended for small strings, say less than 100.
* `PrintStrN(uint16_t size)` is a non-templatized class which creates
  the character array buffer on the heap.
    * The `size` parameter can be either a compile-time or runtime value.
    * The `size` parameter is given in the constructor.
    * The largest `size` is limited by the size of the heap, which is
      closer to the maximum size of static RAM, compared to the size of the
      stack.

### `PrintStrBase`

All `PrintStr` subclasses inherit from the `PrintStrBase` class,
which also inherits from the Arduino `Print` class.

Since all subclasses of `PrintStr` also implement the `Print` interface, you
can write functions that accept a bare `Print` (or a `Print&` reference), and
write the implementation code inside the function as if you were writing to a
normal `Print` object such as the usual `Serial` object.

However, if you need access to the `PrintStrBase.length()` method, which
returns the string length in the current buffer, you need to use the
`PrintStrBase` (or `PrintStrBase&` reference), instead of using the
`Print`, because the `Print` class does not implement that `length()` method.

### `PrintStr<SiZE>` Example

```C++
#include <Arduino.h>
#include <AceCommon.h>
using ace_common::PrintStrBase;
using ace_common::PrintStr;

void buildMessage(PrintStrBase& message) {
  if (message.length() > 0) {
    message.print("; ");
  }
  message.print("x=");
  message.print(3);
  message.print("; s=")
  message.print("a-string");
}

void doSomething(const char* message) {
  // Do something with message
}

void doStuff() {
  PrintStr<30> message;  // <----------- ONLY DiFFERENCE
  buildMessage(message);

  const char* cstr = message.cstr();
  doSomething(cstr);

  message.flush();
  buildMessage(message);
  ...
}
```

### `PrintStrN(size)` Example

Here is the version using `PrintStrN` class:

```C++
#include <Arduino.h>
#include <AceCommon.h>
using ace_common::PrintStrBase;
using ace_common::PrintStrN;

void buildMessage(PrintStrBase& message) {
  if (message.length() > 0) {
    message.print("; ");
  }
  message.print("x=");
  message.print(3);
  message.print("; s=")
  message.print("a-string");
}

void doSomething(const char* message) {
  // Do something with message
}

void doStuff() {
  PrintStrN message(30); // <----------- ONLY DiFFERENCE
  buildMessage(message);

  const char* cstr = message.cstr();
  doSomething(cstr);

  message.flush();
  buildMessage(message);
  ...
}
```

## Alternatives

After I had created this utility, I discovered the PrintString
(https://github.com/RobTillaart/PrintString) library which does something
similar. The difference is that `PrintString` is a thin wrapper around a
`String` object, which uses the heap as the buffer, which can cause heap
fragmentation. The `PrintStr` class uses the stack to completely avoid the heap.

The `PrintStrN` class uses the heap as the buffer and is closer to the
`PrintString`. The difference is that the `PrintStrN` uses a fixed size
allocation on the heap whereas `String` will reallocate to grow the buffer on
the heap, which can lead to heap fragmentation.
