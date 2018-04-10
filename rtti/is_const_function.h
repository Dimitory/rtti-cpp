#pragma once

namespace Dimitory
{
    template <typename T>
    struct is_const_function
    {
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct is_const_function<ReturnType( ClassType::* )(Args...)>
        : std::false_type
    {
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct is_const_function<ReturnType( ClassType::* )(Args...) const>
        : std::true_type
    {
    };
}