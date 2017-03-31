#ifndef MEMORYARRAY_H
#define MEMORYARRAY_H

#include <memory>

#include "AbstractData.h"
#include "Memory.h"

namespace IR {
    class MemoryArray : public Memory
    {
    public:
        MemoryArray(Type type, std::string name, unsigned size);

         int getSizeInMemory() const;

    protected:
        unsigned dataSize;
    };

    typedef std::shared_ptr<MemoryArray> sh_MemoryArray;
}


#endif // MEMORYARRAY_H
