#ifndef DATA_H
#define DATA_H

#include <memory>
#include <string>

namespace IR {
    enum Type{
        Char, Integer, Long
    };

    class Data
    {
    public:
        Data(Type type, std::string name);

        std::string getName() const;
        Type getType() const;

    protected:
        Type dataType; // 1, 2 or 4 byte(s)
        std::string dataName;
    };

    typedef std::shared_ptr<Data> sh_Data;
}


#endif // DATA_H
