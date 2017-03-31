#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <vector>
#include <memory>

#include "../data/Register.h"
#include "../data/Memory.h"

namespace IR {
    const int ASM_TYPE_X86= 1;


    class Instruction
    {
    public:
        Instruction();

        virtual std::string toString() const =0;

        std::string toAsm(int asmType) const;
        virtual std::string toX86() const =0;

        std::vector<sh_Register> getReadRegisterVector() const;
        std::vector<sh_Register> getWroteRegisterVector() const;
        std::vector<sh_Data> getReadMemoryVector() const;
        std::vector<sh_Data> getWroteMemoryVector() const;

    protected:
        std::vector<sh_Register> readRegisterVector;
        std::vector<sh_Register> wroteRegisterVector;

        std::vector<sh_Data> readMemoryVector;
        std::vector<sh_Data> wroteMemoryVector;
    };

    typedef std::shared_ptr<Instruction> sh_Instruction;
}

#endif // INSTRUCTION_H
