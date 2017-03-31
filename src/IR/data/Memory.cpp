#include "Memory.h"

using namespace IR;

Memory::Memory(Type type, std::string name):
    Data(type,name)
{
    asm_basePointerOffset = 0;
}

int Memory::getAsmBasePointerOffset() const
{
    return asm_basePointerOffset;
}

void Memory::setAsmBasePointerOffset(int value)
{
    asm_basePointerOffset = value;
}
