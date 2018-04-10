#pragma once

#include "type.h"
#include "constructor.h"
#include "type_database.h"

namespace Dimitory
{
    template <class Class, typename ... Args>
    struct ConstructorInfo
    {
        static Constructor<Args...> constructor;
        static Constructor<Args...> registerCtor()
        {
            auto& db = TypeDatabase::getInstance();
            auto& typedata = db.types[typeof( Class ).getID()];
            return typedata.addConstructor<Class, Args...>();
        }
    };
}