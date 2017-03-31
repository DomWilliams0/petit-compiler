#pragma once
#ifndef MEMORYARRAY_H
#define MEMORYARRAY_H

#include "Data.h"

class MemoryArray : public Data
{
public:
	MemoryArray(Type type, std::string name, unsigned size) : Data(type, name), dataSize(size) {};

protected:
	unsigned dataSize;
};

typedef std::shared_ptr<MemoryArray> sh_MemoryArray;

#endif // MEMORYARRAY_H
