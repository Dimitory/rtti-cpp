#include "field.h"

using namespace Dimitory;

Field::Field() : 
    type( Type::Invalid ), 
    getter( nullptr ), 
    setter( nullptr )
{

}

Field::Field( const String& name, Type type, Getter getter, Setter setter ) : 
    name( name ), 
    type( type ), 
    getter( getter ), 
    setter( setter )
{

}

bool Field::isValid() const
{
    return getter != nullptr;
}

bool Field::isReadOnly() const
{
    return setter == nullptr;
}

Type Field::getType() const
{
    return type;
}

const String& Field::getName() const
{
    return name;
}

Object Field::getValue( Object& instance ) const
{
    return getter( instance );
}

bool Field::setValue( Object& instance, const Object& newValue ) const
{
    if ( setter && !instance.isConst() )
    {
        setter( instance, newValue );
        return true;
    }
    return false;
}