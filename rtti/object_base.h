#pragma once

namespace Dimitory
{
    class Type;
    class ObjectBase
    {
    public:
        virtual std::shared_ptr<ObjectBase> clone() const = 0;
        virtual Type getType() const = 0;
        virtual void* getPointer() const = 0;
        virtual bool compare( const ObjectBase& rhs ) const = 0;
    };
}