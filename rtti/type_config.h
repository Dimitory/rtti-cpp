#pragma once

#include <xstring>

namespace Dimitory
{
    template <class T>
    using PureType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    using TypeID = unsigned int;
    using String = std::basic_string < char, std::char_traits<char>, std::allocator<char> >;
}