/*
 * MIT License
 * Copyright (c) 2018 Brian T. Park
 */

/**
 * @file printfTo.h
 *
 * Implement a printfTo() that accept formatting strings like printf() and
 * prints the result to the given `Print` object. This is intended to be used
 * on platforms whose `Print` does not provide a `Print.printf()` function, for
 * example, the original Arduino UNO or Nano. I finally got tired of writing
 * multiple lines of SERIAL_PORT_MONITOR.print() for debugging.
 *
 * The implementation uses an fixed size char[] buffer on the stack that is
 * defined to be of length PRINTF_TO_BUF_SIZE, which is current 192. That
 * should be enough for most debugging uses. It would be more robust if the
 * format string was interpreted by the method to avoid that buffer. But this
 * function is intended only for debugging purposes. For "production"
 * deployments, this function should be disabled through an `#if` statement.
 *
 * NOTE: These *must* be implemented as inline function to allow the compiler
 * to remove unused functions from the binary. For some reason, on AVR, ESP8266
 * and ESP32 compilers, link-time-optimization does not seem to work well. If
 * these functions are defined in a .cpp file, they are included in the binary,
 * even if they are not reference at all by anything. This causes the binary to
 * be about 700 (AVR) to 1000 (ESP32) bytes larger in flash memory.
 *
 * It looks like <stdio.h> is the header to get vsnprintf(), so we no longer
 * need to include <Arduino.h> here.
 */

#ifndef ACE_COMMON_PRINTF_TO_H
#define ACE_COMMON_PRINTF_TO_H

#include <stdio.h> // vsnprintf()
#include <stdarg.h> // va_list, va_start(), va_end()

class Print;

namespace ace_common {

/** Maximum size of the internal stack buffer used by printfTo(). */
const int PRINTF_TO_BUF_SIZE = 192;

/** Helper function for implementing the printfTo() function. */
inline void vprintfTo(Print& printer, const char *fmt, va_list args) {
  char buf[PRINTF_TO_BUF_SIZE];
  vsnprintf(buf, PRINTF_TO_BUF_SIZE, fmt, args);
  printer.print(buf);
}

/**
 * A printf() that works on an Arduino `Print` object using the built-in
 * vsnprintf(). Append a '\\n' at the end of the string to print a newline.
 * This is intended to be used on platforms without a built-in `Print.printf()`
 * function, for example, the original Arduino UNO or Nano.
 */
inline void printfTo(Print& printer, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintfTo(printer, fmt, args);
  va_end(args);
}

}

#endif
