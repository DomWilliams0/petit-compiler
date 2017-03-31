#ifndef OPERATOR_H
#define OPERATOR_H

#include <memory>

#include "Instruction.h"
#include "../data/Register.h"

namespace IR {
    class Operator : public Instruction
    {
    public:
       void Operator(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    private:
        sh_Register destination;
        sh_Register firstValue;
        sh_Register secondValue;
    };
}


#endif // OPERATOR_H
