#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H

#include "Data.h"
#include "Element.h"

class Constant : public Data
{
public:
	//                                           ^^^^^^^ to modify?
	Constant(Type type, long value) : Data(type, "const"), dataValue(value){};
	
	long getDataValue() const;

protected:
	long dataValue;
};

typedef std::shared_ptr<Constant> sh_Constant;

#endif // CONSTANT_H
