#include "MemoryArray.h"

using namespace IR;


void MemoryArray::MemoryArray(Type type, std::string name, unsigned size):
    Data(type, name),
    dataSize(size)
{

}
