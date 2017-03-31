#ifndef MEMORYARRAY_H
#define MEMORYARRAY_H

#include <memory>
#include <string>
#include <iostream>

#include "Data.h"

namespace IR {
    class MemoryArray : public Data
    {
    public:
        void MemoryArray(Type type, std::string name, unsigned size);

    protected:
        unsigned dataSize;
    };

    typedef std::shared_ptr<MemoryArray> sh_MemoryArray;
}


#endif // MEMORYARRAY_H
