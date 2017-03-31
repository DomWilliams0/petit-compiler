
#ifndef OPDIV_H_
#define OPDIV_H_

#include "Operator.h"

namespace IR {
    class OpDiv : public Operator
    {
    public:
      void  OpDiv(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPDIV_H_ */
