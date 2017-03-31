

#include "Data.h"

using namespace IR;

Data::Data(Type type, std::string name):
    dataName(name), dataType(type)
{

}

std::string Data::getName() const
{
    return dataName;
}

Type Data::getType() const
{
    return dataType;
}
