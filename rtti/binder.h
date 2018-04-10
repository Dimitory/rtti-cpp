#pragma once

namespace Dimitory
{
    template <typename Class, typename Function>
    auto call( Function function, Object object, const Arguments& args )
        -> decltype(call_<function_traits<Function>::arity, Class>( function, object, args ))
    {
        return call_<function_traits<Function>::arity, Class>( function, object, args );
    }

    template <size_t Arity, typename Class, typename Function>
    auto call_( Function function, Object object, const Arguments& args )
        -> decltype(call__<Class>( std::forward<Function>( function ), object, args, std::make_index_sequence<Arity>() ))
    {
        assert( args.size() >= Arity );
        return call__<Class>( std::forward<Function>( function ), object, args, std::make_index_sequence<Arity>() );
    }

    template <typename Class, typename Function, size_t... I>
    auto call__( Function function, Object object, const Arguments& args, std::index_sequence<I...> )
        -> decltype((object.getValue<Class>().*function)(args[I].getValue<function_traits<Function>::arg<I>::type>()...))
    {
        return (object.getValue<Class>().*function)(args[I].getValue<function_traits<Function>::arg<I>::type>()...);
    }

    template <typename Class, class Function>
    struct function_binder
    {
    private:
        Function function;

    public:
        explicit function_binder( Function&& function ) :
            function( std::forward<Function>( function ) )
        {}

        auto operator()( Object object, const Arguments& args )
            -> decltype(call<Class>( function, object, args ))
        {
            return call<Class>( function, object, args );
        }
    };

    template<typename Class, typename Function> inline
        function_binder<Class, Function> function_bind( Function&& function )
    {
        return (function_binder<Class, Function>( std::forward<Function>( function ) ));
    }
}