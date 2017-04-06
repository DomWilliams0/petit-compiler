#pragma once
#ifndef DATA_H
#define DATA_H

#include "Element.h"
#include <memory>

class Data
{
public:
	Data(Type type, std::string name) : dataName(name), dataType(type) {};

	std::string getName() const;
	Type getType() const;

protected:
	std::string dataName;
	Type dataType; // 1, 2 or 4 byte(s)
};

typedef std::shared_ptr<Data> sh_Data;

#endif // DATA_H