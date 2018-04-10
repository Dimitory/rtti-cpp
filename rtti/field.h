#pragma once

#include <functional>

#include "type.h"
#include "object.h"

namespace Dimitory
{
    class Field
    {
    public:
        using Getter = std::function<Object( const Object& )>;
        using Setter = std::function<void( Object&, const Object& )>;

        Field();
        Field( const String& name, Type type, Getter getter, Setter setter );

        bool isValid() const;
        bool isReadOnly() const;
        Type getType() const;

        const String& getName() const;
        Object getValue( Object& instance ) const;
        bool setValue( Object& instance, const Object& newValue ) const;

    private:
        Type type;
        String name;
        Getter getter;
        Setter setter;
    };
}