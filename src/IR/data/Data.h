#ifndef DATA_H
#define DATA_H

#include <memory>
#include <string>

namespace IR {
    enum Type{
        CHAR  =1,
        INT_32=2,
        INT_64=4
    };

    class Data
    {
    public:
        Data(Type type, std::string name);

        std::string getName() const;
        Type getType() const;
         int getSizeInMemory() const;

    protected:
        Type dataType; // 1, 2 ou 4 octet
        std::string dataName;
    };

    typedef std::shared_ptr<Data> sh_Data;
}


#endif // DATA_H
