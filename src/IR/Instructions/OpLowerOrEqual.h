
#ifndef OPLOWEROREQUAL_H_
#define OPLOWEROREQUAL_H_

#include "Operator.h"

namespace IR {
    class OpLowerOrEqual : public Operator
    {
    public:
      void  OpLowerOrEqual(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPLOWEROREQUAL_H_ */
