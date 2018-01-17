#include <iostream>

#include "car.hpp"
#include "cat.hpp"

// An example of compile-time initialization and concatenation of strings
// using the car_cat header-only libraries

using namespace ltl;

// A (short) list of type name specialisations, using car
template <typename T> constexpr car type_name = "";
template <> constexpr car type_name<int> = "int";
template <> constexpr car type_name<char> = "char";

// Value categories from most pure rval-ish to most real lval-ish
template <typename T,typename U> constexpr car value_category = "prvalue";
template <typename T> constexpr car value_category<T&&,T&&> = "xvalue (rvalue)";
template <typename T> constexpr car value_category<T&&,T&> = "xvalue (glvalue)";
template <typename T> constexpr car value_category<T&,T&> = "lvalue (&)";
template <typename T> constexpr car value_category<T,T&> = "lvalue";

// Concatentate typename and value category using operator+
#define TYPE_VAL_CAT(expr) type_name<std::remove_reference_t<decltype(expr)>> \
                   + ':' + value_category<decltype(expr),decltype((expr))>

int main()
{
    int lval=1;
    int& lvalref=lval;
    int&& rvalref=1;
    constexpr auto prvalue = TYPE_VAL_CAT(1);
    constexpr auto xvalue_rval = TYPE_VAL_CAT(std::move(lval));
    constexpr auto xvalue_glval = TYPE_VAL_CAT(rvalref);
    constexpr auto lvalue_ref = TYPE_VAL_CAT(lvalref);
    constexpr auto lvalue = TYPE_VAL_CAT(lval);
    // Concatenate the constexpr strings using cat() function
    constexpr auto output = cat(prvalue,'\n'
                               ,xvalue_rval,'\n'
                               ,xvalue_glval,'\n'
                               ,lvalue_ref,'\n'
                               ,lvalue);
    std::cout << output;
}
/*
--- stdout ---
int:prvalue
int:xvalue (rvalue)
int:xvalue (glvalue)
int:lvalue (&)
int:lvalue
-------
*/