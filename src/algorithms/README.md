# Algorithms

This directory contains some low-level algorithms which seem to occur in
multiple libraries. It is not the goal of this library to emulate or replace the
C++ Standard Template Library.

## Binary Search

Two templatized binary search functions over an array of elements or an array of
records.

```C++
namespace ace_common {

// Binary search an (abstract) array of records sorted by key.
template<typename X, typename K>
size_t binarySearchByKey(size_t size, const X& x, K&& key);

// Binary search a list of sorted elements.
template<typename X>
size_t binarySearch(const X list[], size_t size, const X& x);

}
```

See [tests/BinarySearchTest.ino](../../tests/BinarySearchTest/) for examples.
