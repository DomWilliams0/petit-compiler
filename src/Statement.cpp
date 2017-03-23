#include <iostream>
#include "Statement.h"

Statement::~Statement()
{

}

Cond::~Cond()
{

}

Iter::~Iter()
{

}

void NullStatement::print() const
{

}

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

void Cond::updateElse(Block *newElse)
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
	std::cout << "{" << std::endl;
	for(size_t i = 0; i < contents->size(); ++i)
	{
		contents->at(i)->print();
		std::cout << std::endl;
	}
	std::cout << "}" << std::endl;
}
