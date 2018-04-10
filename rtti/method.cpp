#include "method.h"
#include "object.h"

using namespace Dimitory;
 
bool Method::isValid() const
{
    return invoker != nullptr;
}

const Method& Method::invalid()
{
    static Method instance;
    return instance;
}
