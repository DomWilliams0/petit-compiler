#include "Register.h"

std::string Register::getAsmRegisterName() const
{
	return asm_registerName;
}

void Register::setAsmRegisterName(const std::string &value)
{
	asm_registerName = value;
}