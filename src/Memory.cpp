#include "Memory.h"

int Memory::getAsmBasePointerOffset() const
{
	return asm_basePointerOffset;
}

void Memory::setAsmBasePointerOffset(int value)
{
	asm_basePointerOffset = value;
}
