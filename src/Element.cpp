#include "Element.h"
#include "Statement.h"

void printType(Type type)
{
	switch(type)
	{
		case INT32:
			std::cout << "int32_t " << std::endl;
			break;
		case INT64:
			std::cout << "int64_t " << std::endl;
			break;
		case CHAR:
			std::cout << "char " << std::endl;
			break;
		case VOID:
			std::cout << "void " << std::endl;
			break;
	}
}

Element::~Element()
{
}

Document::~Document()
{
}

FuncDef:: ~FuncDef()
{
	delete this->block;
}

void VarDecl::print()
{
	printType(varType);
	std::cout << this->identifier << ";";
}
void FuncDecl::print()
{
	printType(functionType);
	std::cout << identifier << "(";

	for(size_t i = 0; i < args.size(); ++i)
	{
		args[i].print();
		std::cout << ",";
	}

	std::cout << ");";
}
void FuncDef::print()
{
	switch(functionType)
	{
		case INT32:
			std::cout << "int32_t ";
			break;
		case INT64:
			std::cout << "int64_t ";
			break;
		case CHAR:
			std::cout << "char ";
			break;
		case VOID:
			std::cout << "void ";
			break;
	}
	std::cout << identifier << "(";
	for(int i = 0; i < args.size(); ++i)
	{
		args[i].print();
	}
	std::cout;
	block->print();
}

void VarDef::print()
{
	printType(value.type);
	std::cout << identifier << " = ";
	value.printValue();
	std::cout << ";";

}

void Document::print()
{
	std::cout << "Document!";
}
