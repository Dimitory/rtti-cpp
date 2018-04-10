#include "type.h"
#include "type_database.h"
#include "field.h"
#include "argument.h"

using namespace Dimitory;

namespace
{ 
    auto &db = TypeDatabase::getInstance();
}

Type::Type() 
    : id( Type::Invalid )
{

}

Type::Type( const Type& rhs )
    : id( rhs.id )
{

}

Type::Type( TypeID id ) : id( id )
{

}

TypeID Type::getID() const
{
    return id;
}

bool Type::isValid() const
{
    return id != Type::Invalid;
}

bool Type::isPointer() const
{
    return db.types[id].isPointer;
}

bool Type::isClass() const
{
    return db.types[id].isClass;
}

Type::operator bool() const
{
    return id != Type::Invalid;
}

bool Type::operator<( const Type& rhs ) const
{
    return id < rhs.id;
}

bool Type::operator>( const Type& rhs ) const
{
    return id > rhs.id;
}

bool Type::operator>=( const Type& rhs ) const
{
    return id >= rhs.id;
}

bool Type::isDerivedFrom( const Type other ) const
{
    auto& base = db.types[id].baseClasses;
    return base.find( other ) != base.end();
}

const std::vector<Field> Type::getFields() const
{
    std::vector<Field> ret;
    auto& fields = db.types[id].fields;
    for ( auto& field : fields )
        ret.emplace_back( field.second );
    return ret;
}

const Field& Type::getField( const String& name ) const
{
    return db.types[id].fields[name];
}

const Method& Type::getMethod( const String& name ) const
{
    return db.types[id].getMethod( name );
}

const std::set<Type>& Type::getBaseClasses() const
{
    return db.types[id].baseClasses;
}

const std::set<Type>& Type::getDerivedClasses() const
{
    return db.types[id].derivedClasses;
}

bool Type::operator!=( const Type& rhs ) const
{
    return id != rhs.id;
}

bool Type::operator==( const Type& rhs ) const
{
    return id == rhs.id;
}

bool Type::operator<=( const Type& rhs ) const
{
    return id <= rhs.id;
}

Type& Type::operator=( const Type& rhs )
{
    id = rhs.id;
    return *this;
}