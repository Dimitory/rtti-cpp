#pragma once

#include "type_config.h"
#include "type_info.h"
#include "type_info_base.h"
#include "type_database.h"
#include "reflection_database.h"
#include "argument.h"
#include "method.h"
#include "field.h"
#include "constructor.h"
#include "constructor_info.h"
#include "arguments.h"
#include "typeof.h"
#include "arguments_packing.h"
#include "binder.h"

#define REGISTER_TYPE(type)                                                                 \
    Dimitory::TypeID Dimitory::TypeInfo<type>::id =                                         \
        Dimitory::TypeInfo<type>::registerType( #type );

#define REGISTER_BASECLASSE(p_class, p_base)                                                \
    Dimitory::TypeID Dimitory::TypeInfoBase<p_class, p_base>::id =                          \
        Dimitory::TypeInfoBase<p_class, p_base>::registerBaseType();


#define REGISTER_CONSTRUCTOR(p_class, ...)                                                  \
    Dimitory::Constructor<##__VA_ARGS__>                                                    \
        Dimitory::ConstructorInfo<p_class, ##__VA_ARGS__>::constructor =                    \
            Dimitory::ConstructorInfo<p_class, ##__VA_ARGS__>::registerCtor();

#define REGISTER_PROPERTY(p_class, p_name, p_getter, p_setter)                       \
{                                                                                           \
    auto& db = TypeDatabase::getInstance();                                                 \
    auto& typedata = db.types[typeof( p_class ).getID()];                                   \
    typedata                                                                                \
        .addField<p_class>( p_name, &##p_class::##p_getter, &##p_class::##p_setter );\
}

#define REGISTER_READONLY_PROPERTY(p_class, p_name, p_getter)                        \
{                                                                                           \
    auto& db = TypeDatabase::getInstance();                                                 \
    auto& typedata = db.types[typeof( p_class ).getID()];                                   \
    typedata.addReadonlyField<p_class>( p_name, &##p_class::##p_getter );           \
}

#define REGISTER_METOD(p_class, p_name, p_method)                                           \
{                                                                                           \
    auto& db = TypeDatabase::getInstance();                                                 \
    auto& typedata = db.types[typeof( p_class ).getID()];                                   \
    typedata.addMethod<p_class>( p_name, &##p_class::##p_method );                          \
}