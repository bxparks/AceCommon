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
 * @file reverse.h
 *
 * Templatized function that reverses the elements of a given array.
 */

#ifndef ACE_COMMON_REVERSE_H
#define ACE_COMMON_REVERSE_H

namespace ace_common {

/**
 * Reverse the given array in-place.
 * @tparam T type of each element in the array
 */
template <typename T>
void reverse(T list[], size_t size) {
  if (size == 0) return;

  T* left = list;
  T* right = &list[size - 1];
  while (left < right) {
    T temp = *left;
    *left = *right;
    *right = temp;
    left++;
    right--;
  }
}

} // ace_common

#endif
