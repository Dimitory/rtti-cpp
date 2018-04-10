#pragma once

#include "type.h"
#include "type_data.h"

namespace Dimitory
{
    template <class T>
    struct TypeInfo
    {
        static TypeID id;
        static bool defined;

        static TypeID registerType( const String& name )
        {
            auto& db = TypeDatabase::getInstance();
            auto id = db.registerType( name );
            return registerType( id, db.types[id] );
        }

        static TypeID registerType( TypeID typeId, TypeData& data )
        {
            if ( id != Type::Invalid )
                return typeId;

            defined = true;
            typedef std::decay<T> Decayed;
            data.isClass = std::is_class<Decayed::type>::value;
            data.isPointer = std::is_pointer<T>::value;
            id = typeId;

            addDefaultConstructor( data );
            return typeId;
        }

    private:
        template<typename U = T>
        static void addDefaultConstructor(
            TypeData &data,
            typename std::enable_if<!std::is_default_constructible<U>::value>::type* = nullptr );
        
        template<typename U = T>
        static void addDefaultConstructor(
            TypeData &data,
            typename std::enable_if<std::is_default_constructible<U>::value>::type* = nullptr );
    };

    template <class T>
    TypeID TypeInfo<T>::id = Type::Invalid;

    template <class T>
    bool TypeInfo<T>::defined = false;

    template<typename T>
    template<typename U>
    void TypeInfo<T>::addDefaultConstructor(
        TypeData &data,
        typename std::enable_if<
        !std::is_default_constructible<U>::value
        >::type*
    )
    {}

    template<typename T>
    template<typename U>
    void TypeInfo<T>::addDefaultConstructor(
        TypeData &data,
        typename std::enable_if<
        std::is_default_constructible<U>::value
        >::type*
    )
    {
        data.addConstructor<T>();
    }
}