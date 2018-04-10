#pragma once

#include <set>
#include <vector>

#include "type_config.h"
#include "arguments.h"
#include "typeof.h"

namespace Dimitory
{
    class Object;
    class Field;
    class Method;

    template<class T>
    class ObjectT;

    class Type
    {
    public:
        using List = std::vector<Type>;
        static const TypeID Invalid = 0;
        
        Type();
        Type( const Type& rhs );
        Type( TypeID id );

        TypeID getID() const;
        bool isValid() const;
        bool isPointer() const;
        bool isClass() const;

        operator bool() const;

        Type& operator=( const Type& rhs );

        template <typename ... Args>
        Object create( Args&& ... args ) const;

        template <typename Class, typename ... Args>
        static ObjectT<Class> createObj( Args&& ... args );

        // these are needed for algorithms to work
        bool operator<( const Type& rhs ) const;
        bool operator>( const Type& rhs ) const;
        bool operator<=( const Type& rhs ) const;
        bool operator>=( const Type& rhs ) const;
        bool operator==( const Type& rhs ) const;
        bool operator!=( const Type& rhs ) const;

        bool isDerivedFrom( const Type other ) const;

        template <class T>
        bool isDerivedFrom() const;

        const std::vector<Field> getFields() const;
        const Field& getField( const String& name ) const;
        const Method& getMethod( const String& name ) const;

        const std::set<Type>& getBaseClasses() const;
        const std::set<Type>& getDerivedClasses() const;

        template <class T>
        static Type get( T&& v );


    private:
        TypeID id;
    };

    template <typename ... Args>
    Object Type::create( Args&& ... args ) const
    {
        auto signature = Invokable::createSignature<Args...>();
        auto& db = TypeDatabase::getInstance();
        auto& ctor = db.types[id].getConstructor<Args...>( signature );
        if ( ctor.isValid() )
            return ctor.invoke( std::forward<Args>( args )... );
        return Object();
    }

    template <typename Class, typename ... Args>
    static ObjectT<Class> Type::createObj( Args&& ... args )
    {
        auto signature = Invokable::createSignature<Args...>();
        auto& db = TypeDatabase::getInstance();
        auto& classType = typeof( Class );
        auto& ctor = db.types[classType.id].getConstructor<Args...>( signature );
        if ( ctor.isValid() )
            return ctor.invoke( std::forward<Args>( args )... );
        return ObjectT<Class>();
    }
    
    template <class T>
    bool Type::isDerivedFrom() const
    {
        return isDerivedFrom( typeof( T ) );
    }

    template <class T>
    Type Type::get( T&& v )
    {
        return typeof( T );
    }
}