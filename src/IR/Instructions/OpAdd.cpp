#include "OpAdd.h"

using namespace IR;

void OpAdd::OpAdd(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister) :
    Operator(resultRegister,firstValueRegister,secondValueRegister)
{

}
