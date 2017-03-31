#include "Register.h"

using namespace IR;

Register::Register(Type type, std::string name):
    Data(type,name)
{
    asm_registerName = "";
}

std::string Register::getAsmRegisterName() const
{
    return asm_registerName;
}

void Register::setAsmRegisterName(const std::string &value)
{
    asm_registerName = value;
}
