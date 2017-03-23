#include <iostream>
#include "Statement.h"

void Cond::print() const
{
	std::cout << "if(";
	condition->print();
	std::cout << ")" << std::endl;
	ifBlock->print();
	if(elseBlock != nullptr)
	{
		std::cout << "else";
		elseBlock->print();
	}

}

void Cond::updateElse(Statement *newElse)
{
	if (elseBlock == nullptr)
		elseBlock = newElse;
}

void Iter::print() const
{
	std::cout << "while(";
	condition->print();
	std::cout << ")" << std::endl;
	iterBlock->print();
}

void Block::print() const
{
	std::cout << "{";
	if (contents != nullptr)
	{
		std::cout << std::endl;
		for(size_t i = 0; i < contents->size(); ++i)
		{
			if (i != 0)
				std::cout << std::endl;
			contents->at(i)->print();
		}
	}
	std::cout << "}" << std::endl;
}

void Return::print() const
{
	std::cout << "return";
	if (value)
	{
		std::cout << " ";
		value->print();
	}

	std::cout << ";";
}
