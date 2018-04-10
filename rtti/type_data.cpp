#include "invokable.h"
#include "type_data.h"
#include "type_database.h"

using namespace Dimitory;

TypeData::TypeData()
    : isPointer( false ), isClass( false )
{

}

TypeData::TypeData( const String& name )
    : name( name ), isPointer( false ), isClass( false )
{

}

const Method& TypeData::getMethod( const String& name ) const
{
    auto it = methods.find( name );
    if ( it == methods.end() )
        return Method::invalid();

    if ( it->second.empty() )
        return Method::invalid();

    return it->second.begin()->second;
}

const Method& TypeData::getMethod( const String& name, const Signature& signature ) const
{
    auto it = methods.find( name );
    if ( it == methods.end() )
        return Method::invalid();

    auto search = it->second.find( signature );

    if ( search == it->second.end() )
        return Method::invalid();

    return search->second;
}

TypeID TypeData::addBaseClass( TypeDatabase& db, Type thisType, Type baseType )
{
    if ( baseType.getID() == Type::Invalid )
        return Type::Invalid;

    baseClasses.insert( baseType );
    db.types[thisType.getID()].derivedClasses.insert( thisType );
    return baseType.getID();
}