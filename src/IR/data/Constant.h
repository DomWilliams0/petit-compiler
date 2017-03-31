#ifndef CONSTANT_H
#define CONSTANT_H

#include <memory>

#include "Data.h"

namespace IR {
    class Constant : public Data
    {
    public:
        Constant(Type type, long value);

        long getDataValue() const;

    protected:
        long dataValue;
    };

    typedef std::shared_ptr<Constant> sh_Constant;
}


#endif // CONSTANT_H
