#ifndef CALL_H
#define CALL_H

#include <memory>
#include <list>

#include "Instruction.h"
#include "../function/Function.h"
#include "../data/Data.h"

namespace IR {
    class Call : public Instruction
    {
    public:
        Call(std::shared_ptr<Function> function, std::list<sh_Data> parameters);

        virtual std::string toString() const;

        virtual std::string toX86() const;

    private:
        std::shared_ptr<Function> calledFunction;
        std::list<sh_Data> functionParam;
    };
}


#endif // CALL_H
