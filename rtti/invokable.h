#pragma once

#include <vector>
#include <xhash>

#include "type.h"
#include "typeof.h"

namespace Dimitory
{
    using Signature = std::vector<Type>;
    class Invokable
    {
    public:
        Invokable( const String& name ) : 
            name( name )
        {}
        const String getName() const
        {
            return name;
        }

        const Signature& getSignature() const
        {
            return signature;
        }

        template <typename ...Types>
        static Signature createSignature()
        {
            static Signature sig;
            static auto init = true;
            if ( init )
            {
                SignaturePacking::Unpack<Types...>::apply( sig );
                init = false;
            }
            return sig;
        }
    protected:
        String name;
        Signature signature;
    };

    namespace SignaturePacking
    {
        template <class... T>
        struct Unpack {};

        template<>
        struct Unpack<>
        {
            static void apply( std::vector<Type>& ) {}
        };

        template <class Head, class... Tail>
        struct Unpack<Head, Tail...>
        {
            static void apply( std::vector<Type>& types )
            {
                types.emplace_back( typeof( Head ) );
                Unpack<Tail...>::apply( types );
            }
        };
    }
}

namespace std
{
    template <>
    struct hash<Dimitory::Signature>
    {
        size_t operator()( const Dimitory::Signature& sig ) const
        {
            size_t seed = 0;
            for ( auto& type : sig )
                seed ^= hash_value( type.getID() ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}