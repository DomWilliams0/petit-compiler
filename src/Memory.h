#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "Data.h"
#include "Element.h"

class Memory : public Data
{
public:
	Memory(Type type, std::string name) : Data(type, name), asm_basePointerOffset(0) {};

	int getAsmBasePointerOffset() const;
	void setAsmBasePointerOffset(int value);

private:
	int asm_basePointerOffset;

};

typedef std::shared_ptr<Memory> sh_Memory;

#endif // MEMORY_H