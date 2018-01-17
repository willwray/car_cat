#include <cassert>
#include <sstream>
#include <typeinfo>

#include "car.hpp"

// constexpr operator== specialisation for char array
template <size_t A, size_t B>
constexpr bool operator==(std::array<char,A> const&,
                          std::array<char,B> const&)
{ return false; }

template <size_t N>
constexpr bool operator==(std::array<char,N> const& a,
                          std::array<char,N> const& b)
{
    for (size_t i=0; i<N; ++i)
        if (a[i]!=b[i])
            return false;
    return true;
}

template <typename T> struct wotype;

int main()
{
    using namespace ltl;

    // Test stream output of car via std::array stream output
    std::stringstream sout;
    sout << car{"test stream out"};
    assert( sout.str() == "test stream out");

    // Test construct from string literal deduces size sans the null-terminator
    car hello = "hello";
    static_assert(hello.size() == 5);
    static_assert(std::is_same_v<decltype(hello), car<5>>);

    // Test constexpr declaration
    constexpr car tchello = "hello";
    // Type check fails on g++<=7.2 as deduction guide does not propagate const
    //static_assert(std::is_same_v<decltype(tchello), car<5> const>);

    constexpr car<5> chello = tchello;
    static_assert(std::is_same_v<decltype(chello), car<5> const>);

    constexpr auto ahello = std::array{'h','e','l','l','o'};
    {
    // Test unpack to structured binding
    // i.e. testing std::tuple_size & std::tuple_element specialisations
    auto [H,E,L,K,O] = hello;
    auto [h,e,l,k,o] = ahello;
    assert(H==h && E==e && L==l && K==k && O==o);
    }
    // Test get<>(car)
    static_assert(std::get<0>(chello) == 'h');

    // Test car comparison with std::array
    static_assert( ahello == chello);

    // Test downcast car<N> to array<char,N>
    //constexpr auto const& hollo = as_array(chello);  //  needs linkage
    static constexpr car<5> shello = tchello;
    constexpr auto const& sharray = as_array(shello);
    //constexpr auto const& shar = *static_cast<std::array<char,5> const*>(&shello);
    static_assert(sharray.data() == shello.data());  // same id
    static_assert(sharray == shello);

    // Test size() call on non-const, non-constexpr car in constexpr context
    // (this is OK for a const member fn only dependent on template args)
    car nul = "";
    if constexpr (nul.size())
        assert(nul.size() == 0);
    else
        assert(nul.size() == 0);

    // Test throw on initialising from non-zero-terminated C-array
    try
    {
        char hello[]{'h','e','l','l','o'};
        car hi = hello;
        assert(hi[0]=='h'); // not reached if initialisation throws (use hi)
        assert(false);      // not reached if initialisation throws, check
    }
    catch (std::invalid_argument& e)
    {
        assert(true);
    }
    catch (...)
    {
        assert(false);
    }
    {
    // Test array method access
    constexpr car a = "a";
    static_assert(std::size(a) == 1);
    static_assert(std::is_same_v<decltype(a)::value_type, char>);
    static_assert(a[0] == 'a');
    static_assert(std::get<0>(a) == 'a');
    static_assert(a.front() == 'a');
    static_assert(*a.data() == 'a');
    static_assert(*a.begin() == 'a');
    }
    {
    // Specify size in template arg, various default initialisation syntax
    constexpr car<1> i {};
    constexpr car<1> j = {};
    constexpr car<1> ii {{}};
    constexpr car<1> jj = {{}};
    static_assert(i[0]=='\0');
    static_assert( i==j && i==ii && i==jj);
    constexpr car<1> func(); // NOT default init - function declaration
    static_assert(std::is_same_v<decltype(func), car<1>()>);

    // Specify size in template arg, various initialisation syntax
    constexpr car<1> a = "a";
    constexpr car<1> b {"a"};
    constexpr car<1> c ("a");
    constexpr car<1> d {{'a'}};
    constexpr car<1> e = {"a"};
    constexpr car<1> f = {{'a'}};
    constexpr car<1> g {{'a','\0'}};
    constexpr car<1> h = {{'a','\0'}};

    static_assert( a == b);
    static_assert( a == c);
    static_assert( a == d);
    static_assert( a == e);
    static_assert( a == f);
    static_assert( a == g);
    static_assert( a == h);

    }
    {
    // Deduce size - here 1, various initialisation syntax
    constexpr car a = "a";
    constexpr car b {"a"};
    constexpr car c ("a");
    constexpr car e = {"a"};
    constexpr car g {{'a','\0'}};
    constexpr car h = {{'a','\0'}};

    assert( a == b);
    assert( a == c);
    assert( a == e);
    assert( a == g);
    assert( a == h);
    }
}
