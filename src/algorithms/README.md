# Algorithms

This directory contains some low-level algorithms which seem to occur in
multiple libraries. It is not the goal of this library to emulate or replace the
C++ Standard Template Library.

## Binary Search

Two templatized binary search functions over an array of elements or an array of
records.

```C++
namespace ace_common {

// Binary search an (abstract) array of records sorted by 'key' where 'key'
// returns the element of type 'X' at index 'i'.
template<typename X, typename K>
size_t binarySearchByKey(size_t size, const X& x, K&& key);

// Binary search an array of sorted elements of type 'X'.
template<typename X>
size_t binarySearch(const X list[], size_t size, const X& x);

}
```

See [tests/BinarySearchTest.ino](../../tests/BinarySearchTest/) for examples.

## Linear Search

Two templatized linear search functions over an array of elements or an array of
records.

```C++
namespace ace_common {

// Linear search an (abstract) array of records sorted by 'key' where 'key'
// returns the element of type 'X' at index 'i'.
template<typename X, typename K>
size_t linearSearchByKey(size_t size, const X& x, K&& key);

// Linear search an array of sorted elements of type 'X'.
template<typename X>
size_t linearSearch(const X list[], size_t size, const X& x);

}
```

See [tests/LinearSearchTest.ino](../../tests/LinearSearchTest/) for examples.

## IsSorted

Two templatized functions which determine if an array of elements or an array of
records is sorted.

```C++
namespace ace_common {

// Check if the (abstract) array of records is sorted by key.
template<typename K>
bool isSortedByKey(size_t size, K&& key);

// Check if an array of elements of type 'X' is sorted.
template<typename X>
bool isSorted(const X list[], size_t size);

}
```

See [tests/IsSortedTest.ino](../../tests/IsSortedTest/) for examples.

## Reverse

Reverse the elements of an array in-place.

```C++
namespace ace_common {

template<typename T>
void reverse(T data[], size_t size);

}
```

See [tests/ReverseTest.ino](../../tests/ReverseTest/) for examples.
