
#ifndef OPSUB_H_
#define OPSUB_H_

#include "Operator.h"

namespace IR {
    class OpSub : public Operator
    {
    public:
      void  OpSub(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPSUB_H_ */
