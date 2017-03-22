/*
 * Element.cpp
 *
 *  Created on: 22 mars 2017
 *      Author: lboucaud
 */

#include "Element.h"
#include "Statement.h"


Element::~Element() {
	// TODO Auto-generated destructor stub
}

Document::Document() {
	// TODO Auto-generated constructor stub

}

Document::~Document() {
	// TODO Auto-generated destructor stub
}
FuncDef:: ~FuncDef()
{
	delete this->block;
}

void VarDecl::print()
{
	switch(this->varType)
	{
		case INT32:
			std::cout<<"int32_t ";
			break;
		case INT64:
			std::cout<<"int64_t ";
			break;
		case CHAR:
			std::cout<<"char ";
			break;
		case VOID:
			std::cout<<"void ";
			break;
	}
	std::cout<<this->identifier<<";"<<std::endl;
}
void FuncDecl::print()
{
	switch(functionType)
	{
		case INT32:
			std::cout<<"int32_t ";
			break;
		case INT64:
			std::cout<<"int64_t ";
			break;
		case CHAR:
			std::cout<<"char ";
			break;
		case VOID:
			std::cout<<"void ";
			break;
	}
	std::cout<<identifier<<"(";
	for(int i=0;i<args.size();++i)
	{
		args[i].print();
		std::cout<<",";
	}
	std::cout<<");"<<std::endl;

}
void FuncDef::print()
{
	switch(functionType)
	{
		case INT32:
			std::cout<<"int32_t ";
			break;
		case INT64:
			std::cout<<"int64_t ";
			break;
		case CHAR:
			std::cout<<"char ";
			break;
		case VOID:
			std::cout<<"void ";
			break;
	}
	std::cout<<identifier<<"(";
	for(int i=0;i<args.size();++i)
	{
		args[i].print();
	}
	std::cout<<std::endl;
	block->print();
}

void VarDef::print()
{
	value.printType();
	std::cout<<identifier<<" = ";
	value.printValue();
	std::cout<<";"<<std::endl;

}
void Document::print()
{

}
