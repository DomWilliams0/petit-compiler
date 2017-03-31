#include "ReadArrayValue.h"

using namespace IR;

void ReadArrayValue::ReadArrayValue(sh_MemoryArray array, sh_Register caseNumber, sh_Register dest):
    Instruction(),
    arraySource(array),
    caseNumberRegister(caseNumber),
    destRegister(dest)
{
    this->readMemoryVector.push_back(arraySource);
    this->readRegisterVector.push_back(caseNumberRegister);
    this->writtenRegisterVector.push_back(destRegister);
}

std::string ReadArrayValue::toString() const
{
    std::string ret = "";
    ret.append( arraySource->getName() );
    ret.append( "[" );
    ret.append( caseNumberRegister->getName() );
    ret.append( "] -> " );
    ret.append( destRegister->getName() );
    return ret;
}

std::string ReadArrayValue::toX86() const
{

}
