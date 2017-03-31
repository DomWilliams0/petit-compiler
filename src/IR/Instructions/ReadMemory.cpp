#include "ReadMemory.h"

using namespace IR;

void ReadMemory::ReadMemory(sh_Memory from, sh_Register to):
    MemoryAcess(),
    source(from),
    destination(to)
{
    this->readMemoryVector.push_back(from);
    this->writtenRegisterVector.push_back(to);
}

std::string ReadMemory::toString() const
{
    std::string ret = "";
    ret.append( source->getName() );
    ret.append( " -> " );
    ret.append( destination->getName() );
    return ret;
}

std::string ReadMemory::toX86() const
{

}
