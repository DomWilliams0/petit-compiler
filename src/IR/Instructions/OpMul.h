
#ifndef OPMUL_H_
#define OPMUL_H_

#include "Operator.h"

namespace IR {
    class OpMul : public Operator
    {
    public:
      void  OpMul(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPMUL_H_ */
