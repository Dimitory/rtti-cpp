#pragma once

#include <type_traits>

namespace Dimitory
{
    namespace equal
    {
        struct no_equal_exists {};
        template<typename T, typename Arg> no_equal_exists operator== ( const T&, const Arg& );

        template<typename T, typename Arg = T>
        struct equal_exists
        {
            enum { value = !std::is_same<decltype(*(T*)(0) == *(Arg*)(0)), no_equal_exists>::value };
        };
    }
}