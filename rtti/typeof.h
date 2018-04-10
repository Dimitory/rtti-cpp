#pragma once

//#define typeof(type)                                                                \
//    Dimitory::Type(Dimitory::TypeInfo<type>::registerType( #type ))

#define typeof(type)                                                                \
    Dimitory::Type(Dimitory::TypeInfo<Dimitory::PureType<type> >::id)