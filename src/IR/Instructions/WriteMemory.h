#ifndef WRITEMEMORY_H
#define WRITEMEMORY_H

#include "MemoryAcess.h"

namespace IR {
    class WriteMemory : public MemoryAcess
    {
    public:
        WriteToMemory(sh_Register from, sh_Memory to);
        WriteToMemory(char value, sh_Memory to);
        WriteToMemory(int value, sh_Memory to);
        WriteToMemory(long value, sh_Memory to);

        virtual std::string toString() const;
        virtual std::string toX86() const;

    private:
        sh_Register source;
        sh_Memory destination;
        int type;
        char charValue;
        int intValue;
        long longValue;

        static const int MEMORY_TYPE;
        static const int CHAR_TYPE;
        static const int INT_TYPE;
        static const int LONG_TYPE;
    };
}


#endif // WRITEMEMORY_H
