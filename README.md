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

