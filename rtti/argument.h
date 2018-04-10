#pragma once

#include "type.h"
#include "object.h"

namespace Dimitory
{
    class Argument
    {
    public:
        Argument() :
            objectBase( nullptr )
        {}

        template <class T>
        Argument( std::unique_ptr<T> data,
            typename std::enable_if<std::is_class<std::unique_ptr<T>>::value>::type* = nullptr ) :
            objectBase( new ObjectContainer<PureType<T>>( std::move( data ) ) )
        {}

        template<class T>
        Argument( T& data,
            typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_reference<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_same<Object&, T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_const<T>::value>::type* = nullptr ) :
            objectBase( new ObjectContainer<PureType<T>>( std::make_unique<T>( data ) ) )
        {}

        template <class T>
        Argument( T&& data,
            typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_reference<T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_same<Object&, T>::value>::type* = nullptr,
            typename std::enable_if<!std::is_const<T>::value>::type* = nullptr ) :
            objectBase( new ObjectContainer<PureType<T>>( std::make_unique<T>( data ) ) )
        {}

        Argument( nullptr_t ) :
            objectBase( nullptr )
        {}

        Argument( const Argument& rhs ) :
            objectBase( rhs.objectBase )
        {

        }

        Argument( Argument&& rhs ) :
            objectBase( rhs.objectBase )
        {}

        ~Argument() {}

        Argument& operator=( Argument&& rhs )
        {
            objectBase = rhs.objectBase;
            return *this;
        }

        Argument& operator=( Argument& rhs )
        {
            objectBase = rhs.objectBase;
            return *this;
        }

        bool operator== ( const Argument& rhs )
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

        bool operator!= ( const Argument& rhs )
        {
            return !(*this == rhs);
        }

        Type getType() const;
        bool isValid() const;;

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
    };
}