#pragma once

#include "object_base.h"
#include "operator_check.h"

namespace Dimitory
{
    template <class T>
    class ObjectContainer : public ObjectBase
    {
    public:
        ObjectContainer( std::unique_ptr<T> value ) : 
            value( std::move( value ) )
        {}
        
        std::shared_ptr<ObjectBase> clone() const override
        {
            //static_assert(!std::is_copy_constructible<T>::value, "Attempting to reference a deleted function");
            return std::make_shared<ObjectContainer<T>>( nullptr );//std::make_shared<ObjectContainer<T>>( std::make_unique<T>( new T( value ) ) );
        }

        bool compare( const ObjectBase& rhs ) const override
        {
            return compareObjectBase( rhs );
        }

        template<typename U = T>
        bool compareObjectBase( const ObjectBase& rhs,
            typename std::enable_if<equal::equal_exists<U, U>::value>::type* = nullptr ) const
        {
            auto a = *value.get();
            auto b = *((const ObjectContainer<T>*)&rhs)->value.get();

            return a == b;
        }

        template<typename U = T>
        bool compareObjectBase( const ObjectBase& rhs,
            typename std::enable_if<!equal::equal_exists<U, U>::value>::type* = nullptr ) const
        {
            return value == ((const ObjectContainer<T>*)&rhs)->value;
        }

        Type getType() const override
        {
            return Type( TypeInfo<PureType<T> >::id );
        }

        void* getPointer() const override
        {
            return value.get();
        }

        ObjectContainer& operator=( const ObjectContainer& ) = delete;
    private:
        std::unique_ptr<T> value;
    };    
}