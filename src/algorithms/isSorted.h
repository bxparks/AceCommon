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
 * @file isSorted.h
 *
 * Templatized functions that determine if an array of things is sorted.
 */

#ifndef ACE_COMMON_IS_SORTED_H
#define ACE_COMMON_IS_SORTED_H

namespace ace_common {

/**
 * Determine if the elements of the array is sorted This function assumes that
 * 'operator<()' for the value type `X` is defined.
 *
 * @tparam X type of element in list
 * @param list sorted array of elements of type X (accepts both const array
 *    or a pointer to the array)
 * @param size number of elements
 */
template<typename X>
bool isSorted(const X list[], size_t size) {
  if (size == 0) return true;

  // Using prev and current temp variables saves about a few bytes of flash
  // compared to using list[i] < list[i - 1].
  X prev = list[0];
  for (size_t i = 1; i < size; ++i) {
    X current = list[i];
    if (current < prev) return false;
    prev = current;
  }
  return true;
}

#if 0
// This shorter alternative runs a lot slower on many platforms because the
// compiler is not able to optimize away the lambda expression and so the
// isSortedByKey() makes a function call on each iteration.
template<typename X>
bool isSorted(const X list[], size_t size) {
  return isSortedByKey(size,
      [&list](size_t i) { return list[i]; } /*key*/);
}
#endif

/**
 * Determine if the abstract array is sorted according to its 'key'. Returns
 * true if sorted, false if not sorted. Returns false if the size of the array
 * is 0.
 *
 * This function assumes that 'operator<()' for the value type of `key` is
 * defined.
 *
 * Performance Note: Many compilers (all?) are not able to opimize away the
 * function call overhead of the lambda expression. If performance is critical,
 * you should copy and modify this code instead.
 *
 * @tparam K lambda expression or function pointer that returns some
 *    unspecified value at index 'i'. The type of the value is inferred
 *    automatically using the 'auto' keyword.
 *
 * @param size number of elements in the array
 * @param key a function or lambda expression that returns the value
 *    at index 'i'. If the 'key' inlined, I think the compiler is smart
 *    enough to inline the 'key' into this code, and avoid a function call.
 */
template <typename K>
bool isSortedByKey(size_t size, K&& key) {
  if (size == 0) return true;

  auto prev = key(0);
  for (size_t i = 1; i < size; ++i) {
    auto current = key(i);
    if (current < prev) return false;
    prev = current;
  }
  return true;
}

/**
 * Same as isSorted() but checks for reverse sorting.
 *
 * @tparam X type of element in list
 * @param list sorted array of elements of type X (accepts both const array
 *    or a pointer to the array)
 * @param size number of elements
 */
template<typename X>
bool isReverseSorted(const X list[], size_t size) {
  if (size == 0) return true;

  // Using prev and current temp variables saves about a few bytes of flash
  // compared to using list[i] < list[i - 1].
  X prev = list[0];
  for (size_t i = 1; i < size; ++i) {
    X current = list[i];
    if (current > prev) return false;
    prev = current;
  }
  return true;
}

/**
 * Same as isSortedByKey() but checks for reverse sorting.
 *
 * @tparam K lambda expression or function pointer that returns some
 *    unspecified value at index 'i'. The type of the value is inferred
 *    automatically using the 'auto' keyword.
 *
 * @param size number of elements in the array
 * @param key a function or lambda expression that returns the value
 *    at index 'i'. If the 'key' inlined, I think the compiler is smart
 *    enough to inline the 'key' into this code, and avoid a function call.
 */
template <typename K>
bool isReverseSortedByKey(size_t size, K&& key) {
  if (size == 0) return true;

  auto prev = key(0);
  for (size_t i = 1; i < size; ++i) {
    auto current = key(i);
    if (current > prev) return false;
    prev = current;
  }
  return true;
}

} // ace_common

#endif
