
#ifndef OPLOWER_H_
#define OPLOWER_H_

#include "Operator.h"

namespace IR {
    class OpLower : public Operator
    {
    public:
      void  OpLower(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPLOWER_H_ */
