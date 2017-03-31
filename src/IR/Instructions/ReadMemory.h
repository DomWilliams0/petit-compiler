#ifndef READMEMORY_H
#define READMEMORY_H

#include "MemoryAcess.h"

namespace IR {
    class ReadMemory : MemoryAcess
    {
    public:
     void   ReadMemory(sh_Memory from, sh_Register to);

        virtual std::string toString() const;
        virtual std::string toX86() const;

    private:
        sh_Memory source;
        sh_Register destination;
    };
}


#endif // READMEMORY_H
