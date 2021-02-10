/*
MIT License

Copyright (c) 2021 Brian T. Park

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

/**
 * @file binarySearch.h
 *
 * Templatized implementation of binary search function over a sorted array of
 * things. Initially, I created a non-templatized binary search function for
 * AceTime/src/ace_time/ZoneRegistrar. Then I found that I needed to write
 * similar functions elsewhere, but I didn't want to keep copy and pasting this
 * code, so it became worthwhile to write a templatized version.
 *
 * A binary search algorithm is conceptually simple, but this article
 * (https://www.solipsys.co.uk/new/BinarySearchReconsidered.html) shows that it
 * is difficult to write a version without bugs. It took me 4 tries:
 *
 * Try 1) There was an infinite loop when searching for a non-existent
 *        zoneId due to the exit condition located at the wrong spot.
 * Try 2) Could not search for x=0 because I was using unsigned
 *        integers for a and b, and the (c-1) expression underflowed to
 *        0xffff.
 * Try 3) An infinite loop caused by incorrect lowerbound, should be
 *        a = c + 1, instead of just a = c.
 * Try 4) Finally got it right, I hope!
 *
 * I'm not an expert in C++ template programming, so there may be
 * inefficiencies in the code below. For example, I assume that the 'X' type
 * will normally be a primative type (e.g. 'int' or 'uint32_t') and will be
 * cheap to copy. This code is "good enough" for my purposes.
 */

#ifndef ACE_COMMON_BINARY_SEARCH_H
#define ACE_COMMON_BINARY_SEARCH_H

#include <stdint.h> // size_t, SIZE_MAX

namespace ace_common {

/**
 * Perform a binary search on the 'records' array of given 'size', which is
 * sorted by the 'key', looking for element 'x' that matches the 'key'. The
 * 'key' is a function or lambda expression to retrieve the value of 'X' at
 * index 'i'.
 *
 * Return the index offset if found. Return SIZE_MAX if not found. SIZE_MAX can
 * never be a valid return value because the largest value for 'size' is
 * SIZE_MAX, which means that largest valid index is 'SIZE_MAX - 1'. Therefore,
 * we can use SIZE_MAX to indicate the 'not found' condition.
 *
 * @tparam R type of each element in records
 * @tparam X type of element to look for, assumed to be cheap to copy (e.g. a
 *    primitive type like an 'int' or 'uint32_t')
 * @tparam K lambda expression or function pointer that returns the 'X' value
 *    at index 'i'
 *
 * @param records an ordered records of R
 * @param size size of elements in records
 * @param x the element to look for
 * @param key a function or lambda expression that returns the 'X' value
 *    at index 'i'. If the 'key' inlined, I think the compiler is smart
 *    enough to inline the 'key' into this code, and avoid a function call.
 *
 * This function assumes that 'operator==()' and 'operator<()' for type 'X' are
 * defined.
 *
 * If the 'records' is not sorted, the function will probably fail to find the
 * element. I don't think it will go into an infinite loop, but I'm not sure.
 *
 * If there are duplicate elements, the function returns the first one that it
 * finds.
 */
template<typename R, typename X, typename K>
size_t binarySearchByKey(
    const R records[], // supports both 'const R[N]' and 'const R*'
    size_t size,
    const X& x,
    K&& key
) {
  size_t a = 0;
  size_t b = size;
  while (true) {
    size_t diff = b - a;
    if (diff == 0) break;

    size_t c = a + diff / 2;
    X current = key(c);
    if (current == x) return c;
    if (x < current) {
      b = c;
    } else {
      a = c + 1;
    }
  }
  return SIZE_MAX;
}

/**
 * Simplified version of binarySearchByKey() where R and X are identical so
 * the 'key' lambda expression just returns the value of the 'list[i]'.
 */
template<typename X>
size_t binarySearch(const X list[], size_t size, const X& x) {
  return binarySearchByKey(
      list, size, x,
      [&list](size_t i) { return list[i]; } /*key*/
  );
}

} // ace_common

#endif
