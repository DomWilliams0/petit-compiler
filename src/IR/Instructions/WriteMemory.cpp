#include "WriteMemory.h"

using namespace IR;

const int WriteMemory::MEMORY_TYPE = 0;
const int WriteMemory::CHAR_TYPE   = 1;
const int WriteMemory::INT_TYPE    = 2;
const int WriteMemory::LONG_TYPE   = 3;

void WriteMemory::WriteMemory(sh_Register from, sh_Memory to) :
   MemoryAcess(), destination(to), source(from), type(MEMORY_TYPE)
{
    this->readRegisterVector.push_back(from);
    this->writtenMemoryVector.push_back(to);
}
void WriteMemory::WriteMemory(char value, sh_Memory to) : MemoryAcess(), charValue(value), type(CHAR_TYPE)
{

}

void WriteMemory::WriteMemory(int value, sh_Memory to) : MemoryAcess(), intValue(value), type(INT_TYPE)
{

}

void WriteMemory::WriteMemory(long value, sh_Memory to) : MemoryAcess(), longValue(value), type(LONG_TYPE)
{

}

std::__cxx11::string WriteMemory::toString() const
{
    std::string ret = "";
    ret.append( source->getName() );
    ret.append( " -> " );
    ret.append( destination->getName() );
    return ret;
}

std::string WriteMemory::toX86() const
{

}
