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

#ifndef ACE_COMMON_PRINT_STR_H
#define ACE_COMMON_PRINT_STR_H

#include <stddef.h> // size_t
#include <Print.h>

namespace ace_common {

/**
 * Base class for all template instances of the PrintStr<SIZE> class. A
 * common base class reduces the code size because only one copy of the core
 * code is included across all possible template instances. Otherwise,
 * PrintStr<10> is a different class than PrintStr<20> and would pull in
 * duplicate copies of the code.
 *
 * Usually the `Print` base class can be used to accept instances of the
 * `PrintStr<SIZE>` objects. However, if you need access to the `length()`
 * method, then you need to use the `PrintStrBase` class instead, since the
 * `Print` class does not have a `length()` method.
 *
 * Here are the actual numbers. I modified tests/CommonTest.ino program to use
 * 2 template instances, PrintStr<10> and PrintStr<20> instead of just
 * PrintStr<10>. Without this base class optimization, the sketch uses:
 *
 *    * 10030 bytes (32%) of program storage space,
 *    * 710 bytes (34%) of dynamic memory on an Arduino Nano.
 *
 * After inserting this PrintStrBase class in the class hierarchy, the same
 * sketch uses:
 *
 *    * 9936 bytes (32%) of program storage space,
 *    * 698 bytes (34%) of dynamic memory,
 *
 * So we save 94 bytes of flash memory, and 12 bytes of static RAM. And even
 * better, the program and RAM size was the same as using 2 PrintStr<10>
 * instances. In other words, the amount of flash and static RAM remains
 * constant no matter how many template instances we create.
 */
class PrintStrBase: public Print {
  public:
    /** Write a single character into the internal buffer. */
    size_t write(uint8_t c) override {
      if (index_ < size_ - 1) {
        buf_[index_] = c;
        index_++;
        return 1;
      } else {
        return 0;
      }
    }

    /** Write the `buf` string of `size` into the internal buffer. */
    size_t write(const uint8_t *buf, size_t size) override {
      if (buf == nullptr) return 0;

      size_t n = 0;
      while (size-- > 0) {
        size_t ret = write(*buf++);
        if (ret == 0) break;
        n++;
      }
      return n;
    }

    /**
     * Clear the internal buffer.
     *
     * On most platforms, the Print::flush() method is virtual, so this would
     * normally be tagged with the 'override' keyword. But on other platforms
     * (e.g. ESP32, ARDUINO_ARCH_STM32 using
     * https://github.com/stm32duino/Arduino_Core_STM32, ARDUINO_AVR_ATTINYX5
     * using https://github.com/SpenceKonde/ATTinyCore, and potentially others),
     * the Print::flush() is not virtual. It is too much work to maintain an
     * updated list of plaforms where this difference exists. Let's just use the
     * lowest-common denominator and not use the 'override' keyword. If the
     * parent class defines as virtual, this will still override the parent due
     * to the traditional behavior of C++. We just have to make sure that on all
     * platforms, we always call PrintStr::flush() directly, (i.e.
     * non-polymorphically), instead of through one of its base classes (either
     * Print or PrintStrBase).
     */
    void flush() /*override*/ {
      index_ = 0;
    }

    /**
     * Return the NUL terminated c-string buffer. After the buffer is no longer
     * needed, the flush() method should be called to reset the internal buffer
     * index to 0.
     */
    const char* cstr() const {
      buf_[index_] = '\0';
      return buf_;
    }

    /** Backwards compatible version of cstr(). New code should use cstr(). */
    const char* getCstr() const { return cstr(); }

    /** Return the length of the internal c-string buffer. */
    size_t length() const { return index_; }

  protected:
    /**
     * Constructor.
     * @param size the maximum size of the given `buf` buffer
     * @param buf pointer to the character buffer created by the subclass. The
     *        buffer will be either on the stack or on the heap.
     */
    PrintStrBase(uint16_t size, char* buf):
        size_(size),
        buf_(buf) {}

  private:
    // Disable copy constructor and assignment operator
    PrintStrBase(const PrintStrBase&) = delete;
    PrintStrBase& operator=(const PrintStrBase&) = delete;

    // These member variables are declared together for more efficient packing
    // on 32-bit processors.
    uint16_t const size_;
    uint16_t index_ = 0;

  protected:
    /**
     * This is the pointer to the character array buffer. For instances of
     * `PrintStr<SIZE>`, this points to `actualBuf_` which is created on the
     * stack. For instances of `PrintStrN`, it points to the char array
     * allocated on the heap.
     *
     * In the case of `PrintStr`, there might be a way to remove this member
     * variable by calculating the pointer to the stack buffer, by extending
     * the pointer from the last element of this object (i.e. &index_), and
     * thereby saving some memory.
     *
     * But I am not convinced that I have the knowledge do that properly
     * without triggering UB (undefined behavior) in the C++ language. Do I
     * define buf_[0] here and will it point exactly where actualBuf_[] is
     * allocated? Or do I use [&PrintStrBase + sizeof(PrintStrBase)] to
     * calculate the pointer to actualBuf_. I just don't know what's actually
     * allowed in the language spec versus something that works by pure luck on
     * a particular microcontroller and compiler. So I'll pay the cost of the 2
     * extra bytes (8-bit) or 4 extra bytes (32-bit processors) of RAM and
     * store the pointer to actualBuf_ explicitly here in the base class.
     */
    char* const buf_;
};

/**
 * An implementation of `Print` that writes to an in-memory buffer supporting
 * strings less than 65535 in length. It is intended to be an alternative to
 * the `String` class to help avoid heap fragmentation due to repeated creation
 * and deletion of small String objects. The 'PrintStr' object inherit the
 * methods from the 'Print' interface which can be used to build an internal
 * string representation of various objects. Instead of using the
 * `operator+=()` or the `concat()` method, use the `print()`, `println()` (or
 * sometimes the `printf()` method) of the `Print` class. After the internal
 * string is built, the NUL-terminated c-string representation can be retrieved
 * using `getCstr()`.
 *
 * This object is expected to be created on the stack instead of the heap
 * to avoid heap fragmentation. The `SIZE` parameter is a compile time constant,
 * to allow the internal string buffer to be created on the stack. The object
 * will be destroyed automatically when the stack is unwound after returning
 * from the function where this is used.
 *
 * An instance of `PrintStr` can be reused by calling the `flush()` method
 * which causes the internal buffer to be cleared to an empty string. An
 * instance of this object could be created statically and reused across
 * different calling sites, but this was not the main intended use of this
 * class.
 *
 * Warning: The contents of `getCstr()` are valid only as long as the
 * PrintStr object is alive. The pointer should never be passed to another
 * part of the program if the PrintStr object is destroyed before the
 * pointer is used.
 *
 * Usage:
 *
 * @verbatim
 * #include <PrintStr.h>
 *
 * using namespace print_str;
 *
 * void fillStringA(PrintStr& message) {
 *   message.print("hello, ");
 *   message.println("world!");
 * }
 *
 * void fillStringB(PrintStr& message) {
 *   message.print("There are ");
 *   message.print(42);
 *   message.println(" apples");
 * }
 *
 * void someFunction() {
 *   PrintStr<32> message;
 *   fillStringA(message)
 *   const char* cstr = message.getCstr();
 *   // do stuff with cstr
 *
 *   message.flush();
 *   fillStringB(message);
 *   cstr = message.getCstr();
 *   // do more stuff with cstr
 * }
 * @endverbatim
 *
 * @tparam SIZE size of internal string buffer including the NUL terminator
 *         character, the maximum is 65535, which means the maximum string
 *         length is 65534 characters.
 */
template <uint16_t SIZE>
class PrintStr: public PrintStrBase {
  public:
    PrintStr(): PrintStrBase(SIZE, actualBuf_) {}

  private:
    char actualBuf_[SIZE];
};

/**
 * An alternate implementation of `PrintStr` that allocates the character
 * array in the heap, instead of the stack. This allows the creation of
 * `PrintStr` which are much larger than the available stack size. For an
 * ESP8266 for example, the maximum stack size is about 4kB, even though there
 * is 80kB of static RAM available.
 *
 * The name `PrintStrN` was the best I could think of that was relatively
 * short, similar to `PrintStr`, and conveyed the idea that the memory is
 * allocated on the heap, which allows the `size` parameter to be a runtime
 * value, instead of a compile-time value.
 *
 * If this object is created dynamically at the beginning of a function, the
 * character array buffer will be created on the heap. At the end of the
 * function, the object will be automatically destroyed, and destruuctor will
 * automatically reclaim the character array on the heap. If no other heap
 * allocation is performed, then this object should cause no heap
 * fragmentation, just like the `PrintStr` object which uses only the stack.
 */
class PrintStrN: public PrintStrBase {
  public:
    /** Create an instance with an internal buffer of `size` on the heap. */
    PrintStrN(uint16_t size):
      PrintStrBase(size, new char[size]) {}

    /**
     * Delete the internal buffer on the heap. This object itself should never
     * be created on the heap, so we don't need to make the destructor virtual.
     */
    ~PrintStrN() {
      delete[] buf_;
    }
};

}

#endif
