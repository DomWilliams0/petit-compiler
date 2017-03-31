
#ifndef OPGREATER_H_
#define OPGREATER_H_

#include "Operator.h"

namespace IR {
    class OpGreater : public Operator
    {
    public:
      void  OpGreater(sh_Register resultRegister, sh_Register firstValueRegister, sh_Register secondValueRegister);
    };
}


#endif /* OPGREATER_H_ */
