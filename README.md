# memory_view

This is an implementation of the Python
[memoryview](https://docs.python.org/3/library/stdtypes.html#memoryview) for C++.

## Exceptions
If exceptions are used, `memory_view.at(size_type n)` and
`memory_view.view(size_type pos = 0, size_type count = npos)`
may throw a `std::out_of_range()` exception.

If exceptions are disabled, which is checked with the compiler macro `__cpp_exceptions`,
instead of throwing an exception, `std::terminate()` is called.

## Internal Representation
The `memory_view` is internally represented as a `pointer` + `size`.

## usage

### Iterators
A `memory_view` provides iterators that supports the C++ named requirement of
[LegacyRandomAccessIterator](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator).

### Capacity
`memory_view` provides the method `.size()` to get the current size.

The shortcut method `.empty()` returns `true` if the view is empty, a.e. contains 0 elements.

The method `.max_size()` returns the maximum size of a memory view object,
this is the same as the maximum value of `memory_view::size_type`.

### Element Access
`memory_view` provides the `[]` operator, for array like access to the elements of a `memory_view`.

The method `.at(size_type n)` works like operator `[]` but with range checking,
when accessing an element out of range a `std::out_of_range()` is thrown [Exceptions](#Exceptions).

The methods `.front()`, `.back()` and `.data()` are provided
and work the same as for `std::array` or `std::string_view`.

### Modifiers
For compatibility with `std::string_view` the methods `.remove_prefix` and `.remove_suffix` are provided.

For extracting parts of a `memory_view` the method `.view(pos = 0, count = npos)` can be used,
it does not modify the object, it returns a new `memory_view`.

When `.view()` is called without any arguments, it returns a copy of the memory view object.

With the first argument `pos`, the beginning can be sliced off.

With the second argument `count` the length of the new `memory_view` can be set,
it is automatically capped to not return data outside of the original `memory_view` object.
