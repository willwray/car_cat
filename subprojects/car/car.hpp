#pragma once

#include <array>

// std::array<char,N> works as compile-time string, pending something better,
// but it is painful to initialise from a string literal - car<N> fixes that.

namespace ltl
{

// car<N> is an array of chars, publicly inherited from std::array<char,N>
// A deduction guide allows convenient construction from string literal:
//                   constexpr car hi = "hello";
template <size_t N>
struct car : std::array<char,N>
{
    using array = std::array<char,N>;
    constexpr car() : array{} { }
   // Constructor from char[N] assumes that the arg is a string literal -
    // it strips a zero-terminator or fails/throws if it is actually nz.
    constexpr car(char const (&lit)[N+1]) : array{copyN(lit)} { }
  private:
    constexpr array copyN(char const (&lit)[N+1])
    {
        if (lit[N]) throw(std::invalid_argument("not zero terminated"));
        array a{};
        for (size_t i=0; i<N; ++i) a[i] = lit[i];
        return a;
    }
};

// Deduction of the +1 string literal length requires this deduction guide
template <size_t N>
car(char const (&)[N]) -> car<N-1>;

// Down-cast free function, for constexpr requires arg has linkage
template <size_t N>
constexpr auto const& as_array(std::array<char,N> const& a) { return a; }

// Stream output for std::array<char> (& car as it is_a array<char>)
template <size_t N>
std::ostream&
operator<<(std::ostream& os, std::array<char,N> const& str)
{
    os << std::string_view{str.data(),N};
    return os;
}

} // namespace ltl

// customize car tuple access, for structured binding &etc.
namespace std {

template <size_t N>
struct tuple_size<ltl::car<N>>
     : tuple_size<typename ltl::car<N>::array> {};

template <size_t i, size_t N>
struct tuple_element<i, ltl::car<N>>
     : tuple_element<i, typename ltl::car<N>::array> {};

}