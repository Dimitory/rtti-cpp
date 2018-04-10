#pragma once

#include <cstdlib>
#include <tuple>
#include <functional>
#include <type_traits>

namespace Dimitory
{
    template <typename T>
    struct function_traits
        : public function_traits<decltype(&T::operator())>
    {
    };

    template <typename ReturnType, typename... Args>
    struct function_traits<ReturnType( Args... )>
    {
        /**
        .. type:: type result_type
        The type returned by calling an instance of the function object type *F*.
        */
        typedef ReturnType result_type;

        /**
        .. type:: type function_type
        The function type (``R(T...)``).
        */
        typedef ReturnType function_type( Args... );

        /**
        .. data:: static const size_t arity
        Number of arguments the function object will take.
        */
        enum { arity = sizeof...(Args) };

        /**
        .. type:: type arg<n>::type
        The type of the *n*-th argument.
        */
        template <size_t i>
        struct arg
        {
            typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        };
    };

    template <typename ReturnType, typename... Args>
    struct function_traits<ReturnType( *)(Args...)>
        : public function_traits<ReturnType( Args... )>
    {
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType( ClassType::* )(Args...)>
        : public function_traits<ReturnType( Args... )>
    {
        typedef ClassType& owner_type;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType( ClassType::* )(Args...) const>
        : public function_traits<ReturnType( Args... )>
    {
        typedef const ClassType& owner_type;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType( ClassType::* )(Args...) volatile>
        : public function_traits<ReturnType( Args... )>
    {
        typedef volatile ClassType& owner_type;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType( ClassType::* )(Args...) const volatile>
        : public function_traits<ReturnType( Args... )>
    {
        typedef const volatile ClassType& owner_type;
    };

    template <typename FunctionType>
    struct function_traits<std::function<FunctionType>>
        : public function_traits<FunctionType>
    {
    };

    template <typename T>
    struct function_traits<T&> : public function_traits<T> {};
    template <typename T>
    struct function_traits<const T&> : public function_traits<T> {};
    template <typename T>
    struct function_traits<volatile T&> : public function_traits<T> {};
    template <typename T>
    struct function_traits<const volatile T&> : public function_traits<T> {};
    template <typename T>
    struct function_traits<T&&> : public function_traits<T> {};
    template <typename T>
    struct function_traits<const T&&> : public function_traits<T> {};
    template <typename T>
    struct function_traits<volatile T&&> : public function_traits<T> {};
    template <typename T>
    struct function_traits<const volatile T&&> : public function_traits<T> {};
}