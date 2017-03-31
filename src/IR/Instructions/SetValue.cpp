#include "SetValue.h"

using namespace IR;

SetValue::SetValue(Constant constant, sh_Register destRegister):
   Instruction(),
    dest(destRegister),
    value(constant)
{
    this->writtenRegisterVector.push_back(destRegister);
}

std::__cxx11::string SetValue::toString() const
{
    std::string ret = "";
    ret.append( std::to_string(value.getDataValue()) );
    ret.append( " -> " );
    ret.append( dest->getName() );
    return ret;
}

std::string SetValue::toX86() const
{

}
