#include "Constant.h"

using namespace IR;

Constant::Constant(Type type, long value):
    Data(type,"!c"), dataValue(value)
{

}

long Constant::getDataValue() const
{
    return dataValue;
}
