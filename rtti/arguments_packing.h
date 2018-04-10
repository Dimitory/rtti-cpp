#pragma once

#include <vector>

#include "function_traits.h"

namespace Dimitory
{
    namespace ArgumentsPacking
    {
        template <typename... T>
        struct Pack {};

        template<>
        struct Pack<>
        {
            static void apply( Arguments& ) {}
        };

        template <typename Head, typename... Tail>
        struct Pack<Head, Tail...>
        {
            static void apply( Arguments& arguments, Head&& value, Tail&& ... tail )
            {
                arguments.push_back( Argument( value ) );
                Pack<Tail...>::apply( arguments, std::forward<Tail>( tail )... );
            }
        };

        template <typename ...Args>
        static Arguments pack( Args&& ... args )
        {
            Arguments arguments;
            Pack<Args...>::apply( arguments, std::forward<Args>( args )... );
            return arguments;
        }
    };
}