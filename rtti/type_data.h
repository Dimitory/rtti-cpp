#pragma once

#include <unordered_map>
#include <unordered_set>

#include "type.h"
#include "field.h"
#include "method.h"
#include "constructor.h"
#include "function_traits.h"
#include "result_of.h"
#include "is_const_function.h"

namespace Dimitory
{
    class TypeDatabase;
    using Constructors = std::unordered_map<Signature, std::unique_ptr<Invokable>>;
    using Fields = std::unordered_map<String, Field>;
    using Methods = std::unordered_map<String, std::unordered_multimap<Signature, Method> >;

    struct TypeData
    {
        String name;
        std::set<Type> baseClasses;
        std::set<Type> derivedClasses;

        TypeData();
        TypeData( const String& name );

        Fields fields;
        Methods methods;
        Constructors constructors;

        template <typename ... Args>
        const Constructor<Args...>& getConstructor( const Signature& signature );

        const Method& getMethod( const String& name ) const;

        const Method& getMethod( const String& name, const Signature& signature ) const;
        
        TypeID addBaseClass( TypeDatabase& db, Type thisType, Type baseType );

        template <class Class, typename ...Args>
        const Constructor<Args...>& addConstructor();

        template <class Class, typename Getter, typename Setter>
        Field addField( const String& name, Getter getter, Setter setter )
        {
            Field field(
                name,
                Type( TypeInfo<PureType<function_traits<Getter>::result_type>>::id ),
                [getter]( const Object& object )
                {
                    return Object( function_bind<Class>( getter )(object, Arguments{}) );
                },
                [setter]( Object& object, const Object& value )
                {
                    // function_bind<Class>( setter )(object, Arguments{ value });
                }
            );

            fields.emplace( name, field );
            return field;
        }

        template <class Class, typename Getter>
        Field addReadonlyField( const String& name, Getter getter )
        {
            Field field(
                name,
                Type( TypeInfo<PureType<function_traits<Getter>::result_type>>::id ),
                [getter]( const Object& object )
                {
                    return  Object( function_bind<Class>( getter )(object, Arguments{}) );
                },
                nullptr
                );

            fields.emplace( name, field );
            return field;
        }

        template <typename Class, typename Function>
        Method addMethod( const String& name, Function function,
            typename std::enable_if<!std::is_same<void, typename function_traits<Function>::result_type>::value>::type* = nullptr )
        {
            auto invoker = [function]( Object& object, Arguments& args )
            {
                return function_bind<Class>( function )(object, args);
            };
            auto method = Method( name, invoker, is_const_function<Function>::value );
            Signature signature;
            //TODO
            //SignaturePacking::Unpack<function_traits<Function>::arg>::apply( signature );
            methods[name].emplace( signature, method );
            return method;
        }

        template <typename Class, typename Function>
        Method addMethod( const String& name, Function function,
            typename std::enable_if<std::is_same<void, typename function_traits<Function>::result_type>::value>::type* = nullptr )
        {
            std::cout << (std::is_same<function_traits<Function>::result_type, void>::value ? "true" : "false") << std::endl;

            auto invoker = [function]( Object& object, Arguments& args )
            {
                function_bind<Class>( function )(object, args);
                return Object();
            };
            auto method = Method( name, invoker, is_const_function<Function>::value );
            Signature signature;
            //TODO
            //SignaturePacking::Unpack<function_traits<Function>::arg>::apply( signature );
            methods[name].emplace( signature, method );
            return method;
        }

        bool isPointer : 1;
        bool isClass : 1;
    };

    template <typename ... Args>
    const Constructor<Args...>& TypeData::getConstructor( const Signature& signature )
    {
        auto s = constructors.find( signature );
        if ( s == constructors.end() ) return Constructor<Args...>::invalid();
        return *(Constructor<Args...>*)s->second.get();
    }

    template <class Class, typename ...Args>
    const Constructor<Args...>& TypeData::addConstructor()
    {
        auto signature = Invokable::createSignature<Args...>();

        const auto func = []( Args&& ... args )
        {
            return std::make_unique<Class>( std::forward<Args>( args )... );
        };

        auto ctor = std::make_unique<Constructor<Args...>>(
            Type( TypeInfo<PureType<Class>>::id ), 
            signature, 
            func 
        );
        constructors.emplace( signature, std::move( ctor ) );

        return getConstructor<Args...>( signature );
    }    
}