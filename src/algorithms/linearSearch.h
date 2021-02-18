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
 * @file linearSearch.h
 *
 * Templatized implementation of linear search function over an array of
 * things, which is not necessarily sorted. If the array is sorted, you should
 * consider using the binarySearchByKey() function instead.
 *
 * Initially, I wrote a version of this for AceTime/src/ace_time/ZoneRegistrar,
 * which got copied to AceTime/src/ace_time/LinkRegistrar, at which point it
 * made sense to extract it into a templatized function to avoid multiple
 * copies.
 *
 * Normally, I wouldn't consider a linear search function to be worthy of
 * extraction into a library but this file is conceptually similar to the
 * binarySearch.h file.
 */

#ifndef ACE_COMMON_LINEAR_SEARCH_H
#define ACE_COMMON_LINEAR_SEARCH_H

#include <stdint.h> // size_t, SIZE_MAX

namespace ace_common {

/**
 * Perform a linear search for element 'x' on an abstract list of records.
 * The 'key' is a function or lambda expression that retrieves the value of 'X'
 * at index 'i' in the records.
 *
 * This function assumes that 'operator==()' for the value type of `key` is
 * defined.
 *
 * Performance Note: Many compilers (all?) are not able to opimize away the
 * function call overhead of the lambda expression. If performance is critical,
 * you should copy and modify this code instead.
 *
 * @tparam X type of element to look for, assumed to be cheap to copy (e.g. a
 *    primitive type like an 'int' or 'uint32_t')
 * @tparam K lambda expression or function pointer that returns the 'X' value
 *    at index 'i'
 */
template<typename X, typename K>
size_t linearSearchByKey(size_t size, const X& x, K&& key) {
  for (size_t i = 0; i < size; ++i) {
    if (x == key(i)) {
      return i;
    }
  }
  return SIZE_MAX;
}


/**
 * Simplified version of linearSearchByKey() where the elements of the array
 * and the searched element are both of type X. So the `key` lambda expression
 * can be just `list[i]`.
 *
 * This function assumes that 'operator==()' for type `X` is defined.
 *
 * @tparam X type of element in list
 * @param list sorted list of elements of type X (accepts both const array
 *    or a pointer to the array)
 * @param size number of elements
 * @param x element to search for
 */
template<typename X>
size_t linearSearch(const X list[], size_t size, const X& x) {
  for (size_t i = 0; i < size; ++i) {
    if (x == list[i]) {
      return i;
    }
  }
  return SIZE_MAX;

#if 0
  // This shorter alternative runs a lot slower on many platforms because the
  // compiler is not able to optimize away the lambda expression and so the
  // linearSearchByKey() makes a function call on each iteration.
  return linearSearchByKey(size, x,
      [&list](size_t i) { return list[i]; } /*key*/);
#endif
}

} // ace_common

#endif
