
#ifndef OPGREATEROREQUAL_H_
#define OPGREATEROREQUAL_H_

#include "Operator.h"

namespace IR {
    class OpGreaterOrEqual : public Operator
    {
    public:
      void  OpGreaterOrEqual(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPGREATEROREQUAL_H_ */
