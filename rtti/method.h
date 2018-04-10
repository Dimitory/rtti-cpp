#pragma once

#include <functional>

#include "type.h"
#include "invokable.h"
#include "arguments.h"
#include "typeof.h"
#include "object.h"

namespace Dimitory
{
    class Method : public Invokable
    {
    public:
        using Invoker = std::function<Object( Object&, Arguments& )>;

        Method() : 
            Invokable( "INVALID" ), 
            isConstMethod( true ),
            invoker( nullptr )
        {}
        
        Method( const String& name, Invoker invoker, bool isConstMethod ) :
            Invokable( name ),
            isConstMethod( isConstMethod ),
            invoker( invoker )
        {
        }

        bool isValid() const;

        bool isConst() const { return isConstMethod; }

        Object invoke( Object& instance, Arguments& args ) const
        {
            return invoker( instance, args );
        }

        static const Method& invalid();
    private:
        Invoker invoker;
        bool isConstMethod;
    };
}