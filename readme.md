# `car_cat`

This is a repo of two little libs `car` and `cat`, in `namespace ltl`.<br>
`car` and `cat` support the use of char arrays as **compile-time** strings:

* [`car`](subprojects/car/readme.md) : a c++17 type for char array initialisation from string literal
* [`cat`](subprojects/cat/readme.md) : a function to concatenate char sequences and return a char array

>**char array** refers to `array<char,N>` or its derived type `car<N>`  
>**char sequence** also includes `char[N]` and `char` (a sequence of 1)

## C++17, plus concepts TS
**Experimental!** Currently only works with recent `g++ -std=c++17 -fconcepts`

* C++17 class template argument deduction is required by `car`
* C++17 fold expressions simplify the implementation of `cat`
* Support for `std::string_view` is desirable
* C++14 is a minimum requirement anyway, for constexpr `std::array`
* Concepts TS `requires` clauses are expressive for bounds checking etc.


### Using Meson-build subprojects for small header-only libs
I like the idea of little libs - code snippets or boost-like proto std types.<br>
I'm trying meson build subprojects and git submodules to support this.

### To build and run the tests
Requires g++ >= 7.1, meson and ninja (optional).

    meson build
    ninja -C build test

