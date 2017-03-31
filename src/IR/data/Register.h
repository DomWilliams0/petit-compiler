#ifndef REGISTER_H
#define REGISTER_H

#include <memory>
#include<string>

#include "Data.h"

namespace IR {
    class Register : public Data
    {
    public:
        Register(Type type, std::string name);

        std::string getAsmRegisterName() const;
        void setAsmRegisterName(const std::string &value);

    private:
        std::string asm_registerName;
    };

    typedef std::shared_ptr<Register> sh_Register;
}

#endif // REGISTER_H
