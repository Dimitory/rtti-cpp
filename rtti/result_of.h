#pragma once

#include <type_traits>

namespace Dimitory
{
    struct LessPrefer {};
    struct Prefer : LessPrefer {};

    struct Fail;

    template <typename F, typename... Args>
    auto result_of_test( Prefer )
        -> decltype(std::invoke( std::declval<F>(), std::declval<Args>()... ));

    template <typename, typename...>
    Fail result_of_test( LessPrefer );


    template <typename T>
    struct result_of_impl { using type = T; };

    template <>
    struct result_of_impl<Fail> {};
    
    template <typename F>
    struct result_of;

    template <typename F, typename... Args>
    struct result_of<F( Args... )>
        : result_of_impl<decltype(result_of_test<F, Args...>( Prefer() ))> {};
}