#ifndef OPMODULUS_H
#define OPMODULUS_H

#include "Operator.h"

namespace IR {
    class OpModulus : public Operator
    {
    public:
       void OperatorModulus(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif // OPERATORMODULUS_H
