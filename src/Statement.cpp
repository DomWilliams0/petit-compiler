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

void Cond::print() const
{
	std::cout << "if(";
	condition->print();
	std::cout << ")" << std::endl;
	ifBlock->print();
	if(elseBlock != NULL)
	{
		std::cout << "else";
		elseBlock->print();
	}

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
	for(size_t i = 0; i < declarations.size(); ++i)
	{
		declarations[i]->print();
		std::cout << std::endl;
	}
	for(size_t i = 0; i < definitions.size(); ++i)
	{
		definitions[i]->print();
		std::cout << std::endl;
	}
	for(size_t i = 0; i < statements.size(); ++i)
	{
		statements[i]->print();
		std::cout << std::endl;
	}
	std::cout << "}" << std::endl;
}
