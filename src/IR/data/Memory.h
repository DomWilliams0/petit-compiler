#ifndef MEMORY_H
#define MEMORY_H

#include <memory>

#include "Data.h"

namespace IR {
    class Memory : public Data
    {
    public:
        Memory(Type type, std::string name);

        int getAsmBasePointerOffset() const;
        void setAsmBasePointerOffset(int value);

    private:
        int asm_basePointerOffset;

    };

    typedef std::shared_ptr<Memory> sh_Memory;
}


#endif // MEMORY_H
