#pragma once

#include <functional>

#include "type.h"
#include "object.h"
#include "invokable.h"
#include "arguments.h"

namespace Dimitory
{
    template <typename ... Args>
    class Constructor : public Invokable
    {
    public:
        using CreateInstance = std::function<Object( Args&& ... )>;

        Constructor() : 
            Invokable( "Invalid" ), 
            type( { Type::Invalid } ), 
            createInstance( nullptr )
        {
        }

        Constructor( Type type, Signature signature, CreateInstance instanceFunc ) :
            Invokable( "constructor" ), 
            type( type ), 
            createInstance( instanceFunc )
        {
            this->signature = signature;
        }

        bool isValid() const
        {
            return createInstance != nullptr;
        }

        Type getType() const
        {
            return type;
        }

        Object invoke( Args&& ... args ) const
        {
            return createInstance( std::forward<Args>(args)... );
        }

        static const Constructor& invalid()
        {
            static Constructor instance;
            return instance;
        }

    private:
        Type type;
        CreateInstance createInstance;
    };
}