#include <iostream>
#include "Statement.h"

Cond::~Cond() {

}

Iter::~Iter() {

}

void Cond::print()
{
	std::cout<<"if(";
	condition->print();
	std::cout<<")"<<std::endl;
	ifBlock->print();
	if(elseBlock!=NULL)
	{
		std::cout<<"else";
		elseBlock->print();
	}
	std::cout<<std::endl;

}
void Iter::print()
{
	std::cout<<"while(";
	condition->print();
	std::cout<<")"<<std::endl;
	iterBlock->print();
	std::cout<<std::endl;
}
void Block::print()
{
	std::cout<<"{"<<std::endl;
	for(int i=0;i<declarations.size();++i)
	{
		declarations[i].print();
	}
	for(int i=0;i<definitions.size();++i)
	{
		definitions[i].print();
	}
	for(int i=0;i<statements.size();++i)
	{
		statements[i].print();
	}
	std::cout<<"}"<<std::endl;
}
