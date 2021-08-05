/*
 * MIT License
 * Copyright (c) 2021 Brian T. Park
 */

/**
 * @file printIntAsFloat.h
 *
 * Print integers as floating point numbers, without using floatin point
 * operations which are expensive on 8-bit processors.
 *
 *  * printUint16AsFloat3To()
 *    * divide an unsigned integer (uint16_t) by 1000 and print the result as a
 *      floating point number to 3 decimal places
 *  * printUint32AsFloat3To()
 *    * divide an unsigned integer (uint32_t) by 1000 and print the result as a
 *      floating point number to 3 decimal places
 */

#ifndef ACE_COMMON_PRINT_INT_AS_FLOAT_H
#define ACE_COMMON_PRINT_INT_AS_FLOAT_H

#include <stdint.h>
#include <Print.h>
#include "printPadTo.h"

namespace ace_common {

  /**
   * Print a uint16 (e.g. 12345U) as a float after dividing by 1000 (i.e.
   * "12.345").
   */
  inline void printUint16AsFloat3To(Print& printer, uint16_t value) {
    uint16_t whole = value / 1000;
    uint16_t frac = value % 1000;
    printer.print(whole);
    printer.print('.');
    printPad3To(printer, frac, '0');
  }

  /**
   * Print a uint32 (e.g. 123456UL) as a float after dividing by 1000 (i.e.
   * "123.456").
   */
  inline void printUint32AsFloat3To(Print& printer, uint32_t value) {
    uint32_t whole = value / 1000;
    uint16_t frac = value % 1000;
    printer.print(whole);
    printer.print('.');
    printPad3To(printer, frac, '0');
  }
}

#endif
