# `car` : a vehicle for string literals

#### `#include "car.hpp"`
<br>`namespace ltl;`

`car` is a standalone header-only mod depending only on ``std::array``.
<br>It is grouped with `cat` in the [`car_cat`](../../readme.md) collection. It requires C++17.

 Mod `car` contains utility class `car` - a 'string constant' character array type,
 <br>'cast' function `as_array()` and `std::ostream` output `operator<<()`.

`car` is short for 'char array'. It is pronounced in a West Country pirate accent.
<br>It is intended as a syntactic convenience for compile-time string initialisation.
<br>Initialisation from string literal deduces size and strips the null terminator:
```c++
    car hi = "hello";  // compare char hi[] = "hello";
```
This deduces `car<5>`, a type publicly derived from `std::array<char,5>`.
<br>Compare with the C-array initialisation deducing `char[6]`, decaying to `char*`.


`car` throws if there is no null terminator (embedded nulls are not checked):

```c++
    char hello[] = {'h','e','l','l','o'};
    car hi = hello;
```
>`terminate called after throwing an instance of`<br>
`'std::invalid_argument' what():  not zero terminated`

This check has no overhead for constexpr initialisation, the intended use case.

## Usage

`car` is a short syntax for compile-time char array initialisation
producing a
<br>`std::array<char>`-derived type with size deduced and no null terminator.

For example, this variable-template definition of `cv_qual<T>` fits within 80 columns:
```c++
template <class T> constexpr car cv_qual = "";
template <class T> constexpr car cv_qual<T const> = " const";
template <class T> constexpr car cv_qual<T volatile> = " volatile";
template <class T> constexpr car cv_qual<T const volatile> = " const"
                                                          " volatile";
```

It is 'syntax sugar'. It is not a full feature string type. It is not an API type.

Because `car` is a `std::array` it can be used where an array can be used.
<br>This saves on adding overloads, as are needed for `std::string_view` say.
<br>For example, this `as_string_view` helper for `std::array` works for `car`,
<br>as does the included `ostream operator<<()`:
```c++
    template <size_t N>
    std::string_view
    as_string_view(std::array<char,N> const& a)
    {
        return {a.data(),N};
    }
```
When required, which should be very rare, you can cast `car` to `std::array`
<br>using the `as_array()` function:
```c++
    static constexpr car car_v = "cast me array";
    constexpr auto& array_ref = as_array(car_v);
```
## Rationale

`std::string_view` is not useful as a compile-time string.
<br>When used as an argument in a constexpr function, constexpr-ness of size is lost
<br>(this may change if a 'constexpr parameter' proposal is accepted).

Our model is the C-array and its convenient copy-initialisation from string literal:

```C++
    using chars = char[];
    chars hi = "hello";  // deduces char[6] with the terminating null
```
The incomplete array type, with unknown bound, avoids the need to specify size.

`char[N]` is not safe or suitable as a constexpr string type:
* It decays to `char*`, erasing the constexpr size.
* It does not copy (except in that one special case above, init from string  lit).
* It includes the null terminator. Or does it?

`std::array<char,N>` addresses these issues.
<br>A null terminator should not be included, because the size is in the type.

But, there is no convenient initialisation of `std::array` from string literal.

Array size cannot be explicitly specified smaller than the literal length, to chop the null:
```c++
    using std::array;
    //array<char,5> hi = "hello";  // compile FAIL
    array<char,6> hi = "hello";  // OK, but only want 5 chars
```
Two-phase initialisation is a total fail. It disallows const / constexpr:
```c++
    array<char,5> hi;
    std::copy_n("hello",5,hi.data());
```
C++17 offers hope, in class template argument deduction, but delivers peril:
```c++
    array hi = "hello";  // C++17 deduces array<char const*,1>
                         // **HAZARD**!!
```
The proposed [N3824](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3824.htm) [`to_array`](http://en.cppreference.com/w/cpp/experimental/to_array) library TS2 helper function converts C-array to std::array.
<br>Necessarily, though, it has to copy the whole array including the null terminator:
```c++
    using std::experimental::to_array;
    auto hi = to_array("hello");   // returns array<char,6>
                                   // we want array<char,5>
```
[`cat`](../../cat/readme.md) strips the null terminator, without checking it, and returns `std::array`:
```c++
    #include "cat.hpp"
    auto hi = cat("hello");     // returns array<char,5>
                                // doesn't check null terminator
```
The similar `car` construct checks and strips the null terminator and returns `car`:
```c++
    auto hi = car{"hello"};     // deduces car<5>
                                // checks the null terminator
```
Explicit `car` declaration is most succinct:
```c++
    car hi = "hello";
```
If needed, `car` can be cast to `array` at zero cost by `as_array(hi)`,
<br>or copied to a std::array with constructor deduction - `std::array ha = hi;`

A user-defined literal would be an alternative. Define one if you want.
```c++
    auto hi = "hello"_cat;  // UDL not provided
```

Inheritance is a lazy way to get an implementation and conversion to array.
<br>It's not guaranteed to work, according to the standard, but works in practice.
<br>

A `car` is a `std::array` - it has no more or less constraint or semantics.
<br>By defining a constructor (from string literal) it loses aggregate initialisation
<br>but its sole purpose is to accept a string literal and strip its null terminator.

Ideally, it would just 'decay' to `std::array<char>` once its job is done.
<br>There's no way to auto-decay, so an `as_array()` free function is included,
<br>effectively a zero-cost, constexpr-compatible, down-cast to the base class.

Do not be tempted to add constructors and stringy member functions to `car`
<br>because then it cannot be used interchangably with a `std::array`.
<br>Define another type, or use `std::string_view` for stringly things.
