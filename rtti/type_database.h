#pragma once

#include <unordered_map>
#include <vector>

#include "type.h"
#include "type_data.h"

namespace Dimitory
{
    class TypeDatabase
    {
    public:
        std::unordered_map<String, TypeID> ids;
        std::vector<TypeData> types;

        TypeID registerType( const String& name );
        static TypeDatabase& getInstance();
    private:
        // which id will be assigned to next type
        TypeID idCounter;
        TypeDatabase();
    };
}