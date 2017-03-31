
#ifndef OPEQUAL_H_
#define OPEQUAL_H_

#include "Operator.h"

namespace IR {
    class OpEqual : public Operator
    {
    public:
      void  OpEqual(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPEQUAL_H_ */
