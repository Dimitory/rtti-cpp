#pragma once

#include "type.h"
#include "type_data.h"

namespace Dimitory
{
    template <class ClassType, class BaseClassType>
    struct TypeInfoBase
    {
        static TypeID id;

        static TypeID registerBaseType()
        {
            static_assert(!std::is_base_of<ClassType, BaseClassType>(), "DeclaredClass must be derived of BaseClass");

            auto& db = TypeDatabase::getInstance();
            const auto& thisType = typeof( ClassType ).getID();
            const auto& baseType = typeof( BaseClassType ).getID();
            return db.types[thisType].addBaseClass( db, thisType, baseType );
        }
    };
}