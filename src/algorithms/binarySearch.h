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
 * 1) There was an infinite loop when searching for a non-existent
 *    zoneId due to the exit condition located at the wrong spot.
 * 2) Could not search for x=0 because I was using unsigned
 *    integers for a and b, and the (c-1) expression underflowed to
 *    0xffff.
 * 3) An infinite loop caused by incorrect lowerbound, should be
 *    a = c + 1, instead of just a = c.
 * 4) Finally got it right, I hope!
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
 * Perform a binary search for element 'x' on an abstract list of records which
 * are sorted by the 'key'. The 'key' is a function or lambda expression that
 * retrieves the value of 'X' at index 'i' in the records.
 *
 * Note that the list of 'records' does *not* need to be given to this
 * function, because the required information is provided by the 'key' which
 * maps the index 'i' to the element of type 'X'. The only extra bit of
 * information needed is the 'size' of the list of records.
 *
 * Returns the index offset if found. Returns SIZE_MAX if not found. SIZE_MAX
 * can never be a valid return value because the largest value for 'size' is
 * SIZE_MAX, which means that largest valid index is 'SIZE_MAX - 1'. Therefore,
 * we can use SIZE_MAX to indicate the 'not found' condition.
 *
 * This function assumes that 'operator==()' and 'operator<()' for type 'X' are
 * defined.
 *
 * If the abstract array is not sorted, the function will probably fail to
 * find the element. I don't think it will go into an infinite loop, but I'm
 * not sure.
 *
 * If there are duplicate elements, the function returns the first one that it
 * finds.
 *
 * Performance Note: Many compilers (all?) are not able to opimize away the
 * function call overhead of the lambda expression. Fortunately, for a binary
 * search, the number of calls to `key` is small compared to the overhead of
 * the algorithm, so the function call overhead is small, like 10% if I recall.
 * But if performance is critical, you should copy and modify this code
 * instead.
 *
 * @tparam X type of element to look for, assumed to be cheap to copy (e.g. a
 *    primitive type like an 'int' or 'uint32_t')
 * @tparam K lambda expression or function pointer that returns the 'X' value
 *    at index 'i'
 *
 * @param size number of elements in the array
 * @param x the element to look for
 * @param key a function or lambda expression that returns the 'X' value
 *    at index 'i'. If the 'key' inlined, I think the compiler is smart
 *    enough to inline the 'key' into this code, and avoid a function call.
 */
template<typename X, typename K>
size_t binarySearchByKey(size_t size, const X& x, K&& key) {
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
 * Simplified version of binarySearchByKey() where the elements of the array
 * and the searched element are both of type X. So the `key` lambda expression
 * can be just `list[i]`.
 *
 * This function assumes that 'operator==()' and 'operator<()' for type 'X' are
 * defined.
 *
 * Performance Note: Many compilers (all?) are not able to opimize away the
 * function call overhead of the lambda expression. Fortunately, for a binary
 * search, the number of calls to the lambda expression is small compared to
 * the overhead of the algorithm, so the function call overhead is small, like
 * 10% if I recall. But if performance is critical, you should copy and modify
 * this code instead.
 *
 * @tparam X type of element in list
 * @param list sorted list of elements of type X (accepts both const array
 *    or a pointer to the array)
 * @param size number of elements
 * @param x element to search for
 */
template<typename X>
size_t binarySearch(const X list[], size_t size, const X& x) {
  return binarySearchByKey(size, x,
      [&list](size_t i) { return list[i]; } /*key*/);
}

} // ace_common

#endif
