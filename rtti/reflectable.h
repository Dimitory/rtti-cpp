#pragma once

namespace Dimitory
{
    class Type;
    class Reflectable
    {
    public:
        virtual ~Reflectable() = default;
        virtual Type getType() const = 0;
        virtual Reflectable* clone() const = 0;
    };
}