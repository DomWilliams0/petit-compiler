
#ifndef OPADD_H_
#define OPADD_H_

#include "Operator.h"

namespace IR {
    class OpAdd : public Operator
    {
    public:
      void  OpAdd(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPADD_H_ */
