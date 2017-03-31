#ifndef SETVALUE_H
#define SETVALUE_H

#include "../data/Constant.h"
#include "Instruction.h"

namespace IR {
    class SetValue : public Instruction
    {
    public:
        SetValue(Constant constant, sh_Register destRegister);

        virtual std::string toString() const;
        virtual std::string toLinuxX86() const;

    private:
        sh_Register dest;
        Constant value;
    };
}


#endif // SETVALUE_H
