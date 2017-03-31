#include "Operator.h"
#include <vector>

using namespace IR;

void Operator::Operator(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister):
    destination(resultRegister), firstValue(firstValueRegister), secondValue(secondValueRegister)
{
    this->readRegisterVector.push_back(firstValueRegister);
    this->readRegisterVector.push_back(secondValueRegister);

    this->wroteRegisterVector.push_back(resultRegister);
}
