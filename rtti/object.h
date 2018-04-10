#pragma once

#include <memory>

#include "type.h"
#include "object_base.h"
#include "reflectable.h"
#include "object_container.h"

namespace Dimitory
{
    template<class T>
    class ObjectT;

    class Object
    {
    public:
        Object::Object() :
            isConstObject( true ),
            objectBase( nullptr )
        {}

        template <class T>
        Object( std::unique_ptr<T> data,
            typename std::enable_if<std::is_class<std::unique_ptr<T>>::value>::type* = nullptr ) :
            isConstObject( std::is_pointer<T>::value && std::is_const<T>::value ),
            objectBase( new ObjectContainer<PureType<T>>( std::move(data) ) )
        {}

        template<class T>
        Object( T& data,
            typename std::enable_if<!std::is_same<Object, T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_reference<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_same<Object&, T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_const<T>::value>::type* = nullptr ) :
            isConstObject( std::is_pointer<T>::value && std::is_const<T>::value ),
            objectBase( new ObjectContainer<PureType<T>>( std::make_unique<T>( data ) ) )
        {}

        template <class T>
        Object( T&& data,
            typename std::enable_if<!std::is_same<Object, T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_reference<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_same<Object&, T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_const<T>::value>::type* = nullptr ) :
            isConstObject( std::is_pointer<T>::value && std::is_const<T>::value ),
            objectBase( new ObjectContainer<PureType<T>>( std::make_unique<T>( data ) ) )
        {}

        Object( nullptr_t ) :
            isConstObject( true ),
            objectBase( nullptr )
        {}


        Object( const Object& rhs ) :
            isConstObject( rhs.isConstObject ),
            objectBase( rhs.objectBase )
        {}

        Object::Object( Object&& rhs ) :
            isConstObject( rhs.isConstObject ),
            objectBase( rhs.objectBase )
        {};

        Object& operator=( Object&& rhs )
        {
            isConstObject = rhs.isConstObject;
            objectBase = std::move( rhs.objectBase );
            return *this;
        }

        Object& operator=( Object& rhs )
        {
            isConstObject = rhs.isConstObject;
            objectBase = rhs.objectBase ? rhs.objectBase->clone() : nullptr;
            return *this;
        }
        
        bool operator== ( const Object& rhs )
        {
            if ( objectBase && rhs.objectBase )
            {
                if ( objectBase == rhs.objectBase )
                    return true;
                else if ( objectBase->getType() == rhs.objectBase->getType() )
                    return objectBase->compare( *rhs.objectBase );
            }

            return objectBase == rhs.objectBase;
        }

        bool operator!= ( const Object& rhs )
        {
            return !(*this == rhs);
        }
        
        Type getType() const
        {
            if ( objectBase )
                return objectBase->getType();
            else
                return Type();
        }

        bool isValid() const
        {
            return objectBase != nullptr;
        }

        bool isConst() const
        {
            return isConstObject;
        }

        template <typename T>
        T& getValue() const
        {
            return *static_cast<T*>(objectBase->getPointer());
        }

        template <typename T>
        ObjectT<T> get() const
        {
            if ( typeof( T ) == getType() )
                return ObjectT<T>( *this );
            else
                return ObjectT<T>();
        }

    private:
        std::shared_ptr<ObjectBase> objectBase;
        bool isConstObject;
    };

    template <class T>
    class ObjectT : public Object
    {
    public:
        ObjectT() :
            Object()
        {}

        ObjectT( const Object& rhs ) :
            Object( rhs )
        {}

        ObjectT( Object&& rhs ) :
            Object( rhs )
        {}

        T* operator->() const
        {
            return getValue<T>();
        }
    };
}